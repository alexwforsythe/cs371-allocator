// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2014
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument
#include <cmath>     // absolute value



// ---------
// Allocator
// ---------

template <typename T, int N>
class Allocator {

    const int SENTINEL_SIZE =   sizeof(int);
    const int T_SIZE =          sizeof(T);


    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

    private:
        // ----
        // data
        // ----

        char a[N];

        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * <your documentation>
         */
        bool valid () const {
            // traverse sentinel nodes and check to make sure all have valid pairs
            int current_idx = 0;
            int current_node = view(0), pair_node, pair_idx;
            while (current_idx != N) {
                pair_idx = current_idx + abs(current_node) + SENTINEL_SIZE;
                pair_node = view(pair_idx);
                // std::cout << "current_idx is " << current_idx << std::endl;
                // std::cout << "current_node is " << current_node << std::endl;
                // std::cout << "pair_idx is " << pair_idx << std::endl;
                // std::cout << "pair_node is " << pair_node << std::endl;
                // std::cout << std::endl;
                if (current_node != pair_node) {
                    std::cout << "current_idx is " << current_idx << std::endl;
                    std::cout << "current_node is " << current_node << std::endl;
                    std::cout << "pair_idx is " << pair_idx << std::endl;
                    std::cout << "pair_node is " << pair_node << std::endl;
                    std::cout << std::endl;
                    return false;
                }
                current_idx += abs(current_node) + 2 * SENTINEL_SIZE;
                current_node = view(current_idx);
            }

            return true;}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        int& view (int i) {
            return *reinterpret_cast<int*>(&a[i]);}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
        */

        Allocator () {
            if (N < T_SIZE + (2 * SENTINEL_SIZE)) {
                throw std::bad_alloc();
            }

            int block_size = N - (2 * SENTINEL_SIZE);
            view(0) = block_size;
            view(N - SENTINEL_SIZE) = block_size;

            assert(valid());
        }


        Allocator (const Allocator& other) :
            a (other.a)
        { assert(valid()); }

        ~Allocator() {}

        Allocator& operator = (const Allocator& other) {
            a = other.a;
            assert(valid());
            return *this;
        }

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * return 0, if allocation fails
         */
        pointer allocate (size_type n) {
            int valid_size = T_SIZE + (2 * SENTINEL_SIZE);
            int min_size = (n * T_SIZE) + (2 * SENTINEL_SIZE);

            // check precondition
            if (n <= 0 || n > N || min_size > N) {
                throw std::bad_alloc();
            }

            difference_type i = 0;
            while (i < N) {
                int& val = view(i);

                // val is non-negative and big enough
                if (val > 0 && val >= n) {
                    
                    int new_val, new_start, new_end, old_val, old_start, old_end;

                    new_start = i;

                    // not enough space for another allocation, so
                    // allocate the whole block
                    if (val < min_size + valid_size) {
                        new_val = val;
                        new_end = i + new_val + SENTINEL_SIZE;
                    }
                    else {
                        // only allocate as many bytes as requested
                        new_val = n * T_SIZE;
                        new_end = i + new_val + SENTINEL_SIZE;


                        // create/update sentinel of remainder of block
                        old_val = val - (2 * SENTINEL_SIZE) - new_val;
                        old_start = new_end + SENTINEL_SIZE;
                        old_end = old_start + old_val + SENTINEL_SIZE;
                        view(old_start) = old_val;
                        view(old_end) = old_val;
                    }

                    view(new_start) = -new_val;
                    view(new_end) = -new_val;

                    assert(valid());

                    return reinterpret_cast<T*>(&a[new_start + SENTINEL_SIZE]);   
                }
                // either block is already occupied, or no space
                // increment i to next block
                i += abs(val) + (2 * SENTINEL_SIZE);
            }
            assert(valid());

            // not enough space
            // throw std::bad_alloc();
            if (i >= N)
                throw std::bad_alloc();
        }


        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------


        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * <your documentation>
         */


        /* 
            THIS IS THE OLD DEALLOCATE
            IT'S TRASH.
            REWROTE IT AND NOW IT WORKS.

        void deallocate (pointer p, size_type s) {
            // check precondition
            if (p <= 0) {
                throw std::invalid_argument("");
            }

            // std::cout << "pointer being deallocated is " << p << std::endl;

            bool block_begins_heap = false, block_ends_heap = false;

            char* c = reinterpret_cast<char*>(p); //char* equivalent of p

            char* c1 = c - SENTINEL_SIZE;
            int* sent_1 = reinterpret_cast<int*>(c1);
            // std::cout << "sent_1: " << *sent_1 << std::endl;

            char* c2 = c + abs(*(sent_1));
            int* sent_2 = reinterpret_cast<int*>(c2);
            // std::cout << "sent_2: " << *sent_2 << std::endl;

            //make sure we have the right sentinel nodes
            assert(*sent_1 == *sent_2);

            //check left sentinel (if applicable)
            char* beginning = &a[0];
            if (c1 == beginning) {
                std::cout << "left is beginning of heap " << std::endl;
                block_begins_heap = true;
            }
            // std::cout << "beginning of heap: " << *reinterpret_cast<int*>(beginning) << std::endl;


            char* end = &a[N - SENTINEL_SIZE];
            if (c2 == end) {
                std::cout << "right is end of heap " << std::endl;
                block_ends_heap = true;
            }
            // std::cout << "end of heap: " << *reinterpret_cast<int*>(end) << std::endl;

            //no coaslescing necessary
            if (block_begins_heap && block_ends_heap) {
                *(sent_1) = -*(sent_1);
                *(sent_2) = -*(sent_2);

                // std::cout << "new values are " << *sent_1 << " and " << *sent_2 << std::endl;

                assert(valid());
            }
            //coalesce block to the right
            else if (block_begins_heap && !block_ends_heap) {
                // std::cout << "coalescing block to right" << std::endl;
                int* right_sent = reinterpret_cast<int*>(c2 + SENTINEL_SIZE);
                int right_val = *right_sent;

                // std::cout << "value of right is " << right_val << std::endl;

                if (right_val > 0) {
                    // int* new_end = right_sent + right_val + SENTINEL_SIZE;
                    int* new_end = reinterpret_cast<int*>(c2 + right_val + 2*SENTINEL_SIZE);
                    int new_val = -(*sent_1) + right_val + 2 * SENTINEL_SIZE;

                    *new_end = new_val; //this seg faults if new_end is computed AFTER casting to int*
                    *sent_1 = new_val;

                    // std::cout << "new val is " << new_val << std::endl;

                    // int end_idx = (reinterpret_cast<intptr_t>(new_end) - reinterpret_cast<intptr_t>(&a[0])) / 4;
                    // std::cout << "end index is " << end_idx << std::endl;
                    assert(valid());
                }
                else {      //no coalescing required
                    *(sent_1) = -*(sent_1);
                    *(sent_2) = -*(sent_2);
                }
                assert(valid());
            }
            //coalesce block to the left
            else if (!block_begins_heap && block_ends_heap) {
                // std::cout << "coalescing block to left" << std::endl;
                int* left_sent = reinterpret_cast<int*>(c1 - SENTINEL_SIZE);
                int left_val = *left_sent;

                if (left_val > 0) {
                    int* new_start = reinterpret_cast<int*>(c1 - 2*SENTINEL_SIZE - left_val);
                    int new_val = -(*sent_1) + left_val + 2*SENTINEL_SIZE;

                    *new_start = new_val;
                    *sent_2 = new_val;
                }
                else {
                    *(sent_1) = -*(sent_1);
                    *(sent_2) = -*(sent_2);
                }
                assert(valid());
            }
            //coalesce both blocks
            else {
                std::cout << "coalescing both blocks" << std::endl;

                int* right_sent = reinterpret_cast<int*>(c2 + SENTINEL_SIZE);
                int right_val = *right_sent;

                int* left_sent = reinterpret_cast<int*>(c1 - SENTINEL_SIZE);
                int left_val = *left_sent;

                if (left_val > 0 && right_val > 0) {
                    std::cout << "value of left_val is " << left_val << std::endl;
                    std::cout << "value of right_val is " << right_val << std::endl;

                    // char* new_start_c = c1 - 2*SENTINEL_SIZE - left_val;
                    // char* new_end_c = c2 + right_val + 2*SENTINEL_SIZE;

                    // int* new_start = reinterpret_cast<int*>(new_start_c);
                    // int* new_end = reinterpret_cast<int*>(new_end_c);

                    int* new_start = reinterpret_cast<int*>(c1 - 2*SENTINEL_SIZE - left_val);
                    int* new_end = reinterpret_cast<int*>(c2 + right_val + 2*SENTINEL_SIZE);

                    // int* new_end = reinterpret_cast<int*>(c2 + right_val + SENTINEL_SIZE);


                    int new_val = -(*sent_1) + left_val + right_val + 2*SENTINEL_SIZE;
                    
                    *new_start = new_val;
                    *new_end = new_val;
                    std::cout << "pointer difference is " << new_end - new_start << std::endl;
                    std::cout << "new values are " << new_val << std::endl;

                }
                else if (left_val > 0) {
                    int* new_start = reinterpret_cast<int*>(c1 - 2*SENTINEL_SIZE - left_val);
                    int new_val = -(*sent_1) + left_val + 2*SENTINEL_SIZE;

                    *new_start = new_val;
                    *sent_2 = new_val;
                    // std::cout << "new values are " << new_val << std::endl;

                }
                else if (right_val > 0) {
                    int* new_end = reinterpret_cast<int*>(c2 + right_val + 2*SENTINEL_SIZE);
                    int new_val = -(*sent_1) + right_val + 2 * SENTINEL_SIZE;

                    *new_end = new_val;
                    *sent_1 = new_val;
                    // std::cout << "new values are " << new_val << std::endl;
                }
                else {
                    //no coalescing
                    *(sent_1) = -*(sent_1);
                    *(sent_2) = -*(sent_2);
                    // std::cout << "new values are " << -*(sent_1) << " and " << -*(sent_2) << std::endl;
                }
                // assert(valid());
            }
            std::cout << std::endl;
            assert(valid());
        }
        */
         

        void deallocate (pointer p, size_type) {

            //get sentinel of block to be freed
            char* block_start = reinterpret_cast<char*>(p);

            //index of sentinel in heap array
            int left_sent_idx = std::distance(a, block_start - SENTINEL_SIZE);
            int* left_sent = &view(left_sent_idx);

            //if block is already free, throw error?
            if (*left_sent > 0) {
                //throw error here
                throw std::invalid_argument("can't deallocate a block that's already free");
            }
            
            //value to be written to new sentinels at end
            int new_val = 0;
            new_val += abs(*left_sent);

            //get right sentinel
            int right_sent_idx = left_sent_idx + SENTINEL_SIZE + new_val;
            int* right_sent = &view(right_sent_idx);

            //make sure you have the right sentinels
            assert(*left_sent == *right_sent);

            //initialize new_start and new_end indices to be indices of left and right sents of current block
            int new_start_idx = left_sent_idx;
            int new_end_idx = right_sent_idx;


            //left is not at front of heap, so has block to left
            if (left_sent_idx != 0) {
                //get sentinel of neighbor to left
                int* left_neighbor_sent = --left_sent;

                //coalesce this block: keep track of new index (index of left neighbors' left sent)
                //add freed space (plus 2 sents worth of space) to total val to write to new sents at end
                if (*left_neighbor_sent > 0) {
                    new_val += (*left_neighbor_sent) + (2 * SENTINEL_SIZE);
                    new_start_idx -= ((*left_neighbor_sent) + (2 * SENTINEL_SIZE));
                }
            }

            //right is not end of heap, so has block to right
            //same thing as above
            if (right_sent_idx != (N - SENTINEL_SIZE)) {
                int* right_neighbor_sent = ++right_sent;
                //coalesce
                if (*right_neighbor_sent > 0) {
                    new_val += ((*right_neighbor_sent) + (2 * SENTINEL_SIZE));
                    new_end_idx += ((2 * SENTINEL_SIZE) + (*right_neighbor_sent));
                }
            }

            // make sure our new indices are in bounds
            assert (left_sent_idx >= 0 && left_sent_idx <= (N - SENTINEL_SIZE));
            assert (right_sent_idx >= 0 && right_sent_idx <= (N - SENTINEL_SIZE));

            // value of new sentinels to new_val
            view(new_start_idx) = new_val;
            view(new_end_idx) = new_val;
      

            assert(valid());
        }


        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         * throw an invalid_argument exception, if pointer is invalid
         * <your documentation>
         */
        void destroy (pointer p) {
            // TODO: check precondition
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        const int& view (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

#endif // Allocator_h
