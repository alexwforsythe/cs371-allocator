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
#include <cmath>      // absolute value



// ---------
// Allocator
// ---------

template <typename T, int N>
class Allocator {

    // #define SENTINEL_SIZE           sizeof(int)
    // #define T_SIZE                  sizeof(T)
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
            // <your code>

            //traverse sentinel nodes and check to make sure all have valid pairs
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
                    // std::cout << "current_idx is " << current_idx << std::endl;
                    // std::cout << "current_node is " << current_node << std::endl;
                    // std::cout << "pair_idx is " << pair_idx << std::endl;
                    // std::cout << "pair_node is " << pair_node << std::endl;
                    // std::cout << std::endl;
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
            // <your code>
            if (N < T_SIZE + (2 * SENTINEL_SIZE)) {
                std::bad_alloc exception;
                throw exception;
            }

            int block_size = N - (2 * SENTINEL_SIZE);
            view(0) = block_size;
            view(N - SENTINEL_SIZE) = block_size;

            assert(valid());
        }


        Allocator (const Allocator& other) {

        }

        ~Allocator() {

        }

        Allocator& operator = (const Allocator&) {
            
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
            assert(n > 0);
            // std::cout << "value being allocated is " << n << std::endl;
            int valid_size = T_SIZE + (2 * SENTINEL_SIZE);
            int min_size = (n * T_SIZE) + (2 * SENTINEL_SIZE);

            int i = 0;
            int val;
            while (i < N) {
                val = view(i);

                // val is non-negative and big enough
                if (val >= min_size) {
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

                    // std::cout << "new value is " << -new_val << std::endl;
                    // std::cout << std::endl;
                    view(new_start) = -new_val;
                    view(new_end) = -new_val;

                    assert(valid());
                    return reinterpret_cast<T*>(&a[i + SENTINEL_SIZE]);
                }

                // either block is already occupied, or no space
                // increment i to next block
                i += abs(val) + (2 * SENTINEL_SIZE);
            }
            assert(valid());

            // not enough space
            // TODO: or throw bad_alloc?
            // std::bad_alloc exception;
            // throw exception;
            return 0;}

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
        void deallocate (pointer p, size_type) {
            // <your code>
            assert(valid());}

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
