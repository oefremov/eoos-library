/**
 * Buffer class in static and dynamic specializations.
 *
 * This class has a primary template and a partial specialization of the template.
 * The non-specialized template defines a realization that contains a whole buffer,
 * which is defined by a template argument, as data member of the class.
 * The specialization allocates necessary memory size for containing the buffer
 * in a heap memory.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2018, Sergey Baigudin, Baigudin Software
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_BUFFER_HPP_
#define LIBRARY_BUFFER_HPP_

#include "library.AbstractBuffer.hpp"

namespace local
{
    namespace library
    {
        /**
         * Primary template implements the static buffer class.
         *
         * @param T - data type of buffer element.
         * @param L - maximum number of buffer elements, or 0 for dynamic allocation.
         * @param A - heap memory allocator class.
         */
        template <typename T, int32 L, class A = Allocator>
        class Buffer : public library::AbstractBuffer<T,A>
        {
            typedef library::AbstractBuffer<T,A> Parent;

        public:

            /**
             * Constructor.
             */
            Buffer() : Parent(L),
                buf_ (arr_){
            }

            /**
             * Constructor.
             *
             * NOTE: A passed illegal element will be copied to an internal data of the class
             *
             * @param illegal - an illegal value.
             */
            Buffer(const T& illegal) : Parent(L, illegal),
                buf_ (arr_){
            }

            /**
             * Destructor.
             */
            virtual ~Buffer()
            {
            }

            /**
             * Assignment operator.
             *
             * If the source buffer is greater than this buffer,
             * only cropped data of that will be copied.
             *
             * @param buf - reference to source buffer.
             * @return reference to this object.
             */
            Buffer& operator=(const Buffer<T,L,A>& buf)
            {
                this->copy(buf);
                return *this;
            }

            /**
             * Assignment operator.
             *
             * If the source buffer is greater than this buffer,
             * only cropped data of that will be copied.
             *
             * @param buf - reference to source buffer.
             * @return reference to this object.
             */
            Buffer& operator=(const AbstractBuffer<T,A>& buf)
            {
                this->copy(buf);
                return *this;
            }

        protected:

            /**
             * Returns a pointer to the fist buffer element.
             *
             * @return pointer to buffer, or NULL.
             */
            virtual T* getBuffer() const
            {
                return buf_;
            }

        private:

            /**
             * Copy constructor.
             *
             * @param obj - reference to source object.
             */
            Buffer(const Buffer& obj);

            /**
             * Current array of T elements.
             */
            T arr_[L];

            /**
             * Pointer to current array.
             *
             * NOTE: The variable has been defined only for giving the getBuffer member function to be constant.
             */
            T* buf_;

        };

        #ifdef EOOS_NO_STRICT_MISRA_RULES

        /**
         * Partial specialization of the template implements the dynamic buffer class.
         *
         * @param T - data type of buffer element.
         * @param A - heap memory allocator class.
         */
        template <typename T, class A>
        class Buffer<T,0,A> : public AbstractBuffer<T,A>
        {
            typedef library::AbstractBuffer<T,A> ParentSpec1;

        public:

            /**
             * Constructor.
             *
             * @param length - count of buffer elements.
             */
            explicit Buffer(int32 const length) : ParentSpec1(length),
                buf_       (NULL),
                isDeleted_ (true){
                const bool isConstructed = construct(length);
                this->setConstructed( isConstructed );
            }

            /**
             * Constructor.
             *
             * NOTE: A passed illegal element will be copied to an internal data of the class
             *
             * @param length  - count of buffer elements.
             * @param illegal - illegal value.
             */
            Buffer(int32 const length, const T& illegal) : ParentSpec1(length, illegal),
                buf_       (NULL),
                isDeleted_ (true){
                const bool isConstructed = construct(length);
                this->setConstructed( isConstructed );
            }

            /**
             * Constructor.
             *
             * NOTE: Given external buffer has to exist until this object is alive.
             *
             * @param length - number of elements.
             * @param buf    - pointer to external buffer.
             */
            Buffer(int32 const length, T*  const buf) : ParentSpec1(length),
                buf_       (buf),
                isDeleted_ (false){
                const bool isConstructed = construct(length);
                this->setConstructed( isConstructed );
            }

            /**
             * Constructor.
             *
             * NOTE 1: Given external buffer has to exist until this object is alive.
             * NOTE 2: A passed illegal element will be copied to an internal data of the class.
             *
             * @param length  - number of elements.
             * @param buf     - pointer to external buffer.
             * @param illegal - illegal value.
             */
            Buffer(int32 const length, T* const buf, const T& illegal) : ParentSpec1(length, illegal),
                buf_       (buf),
                isDeleted_ (false){
                const bool isConstructed = construct(length);
                this->setConstructed( isConstructed );
            }


            /**
             * Destructor.
             */
            virtual ~Buffer()
            {
                if( isDeleted_ == true )
                {
                    A::free(buf_);
                }
            }

            /**
             * Assignment operator.
             *
             * If the source buffer is greater than this buffer,
             * only cropped data of that will be copied.
             *
             * @param buf - reference to source buffer.
             * @return reference to this object.
             */
            Buffer& operator=(const Buffer<T,0,A>& buf)
            {
                this->copy(buf);
                return *this;
            }

            /**
             * Assignment operator.
             *
             * If the source buffer is greater than this buffer,
             * only cropped data of that will be copied.
             *
             * @param buf - reference to source buffer.
             * @return reference to this object.
             */
            Buffer& operator=(const AbstractBuffer<T,A>& buf)
            {
                this->copy(buf);
                return *this;
            }

        protected:

            /**
             * Returns a pointer to the fist buffer element.
             *
             * @return pointer to buffer, or NULL.
             */
            virtual T* getBuffer() const
            {
                T* buf;
                if( not ParentSpec1::isConstructed() )
                {
                    buf = NULL;
                }
                else
                {
                    buf = buf_;
                }
                return buf;
            }

        private:

            /**
             * Constructor.
             *
             * @param length - count of buffer elements.
             * @return boolean result.
             */
            bool construct(size_t const length)
            {
                bool res;
                if( ParentSpec1::isConstructed() )
                {
                    if(buf_ == NULL)
                    {
                        void* const addr = A::allocate(length * (sizeof(T)));
                        buf_ = reinterpret_cast<T*>( addr );
                    }
                    res = buf_ != NULL;
                }
                else
                {
                    res = false;
                }
                return res;
            }

            /**
             * Copy constructor.
             *
             * @param obj - reference to source object.
             */
            Buffer(const Buffer& obj);

            /**
             * Pointer to external given or self created array.
             */
            T* buf_;

            /**
             * Deletet flag.
             *
             * Is set to true for self created array or
             * set to false for external given array.
             */
            bool isDeleted_;

        };

        #endif // EOOS_NO_STRICT_MISRA_RULES

    }
}
#endif // LIBRARY_BUFFER_HPP_
