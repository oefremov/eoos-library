/** 
 * Abstract class for some buffers.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2016, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ABSTRACT_BUFFER_HPP_
#define LIBRARY_ABSTRACT_BUFFER_HPP_

#include "library.Object.hpp"
#include "api.Collection.hpp"
#include "api.IllegalValue.hpp"

namespace global
{
    namespace library
    { 
        /** 
         * @param T data type of buffer element.
         * @param A heap memory allocator class.
         */ 
        template <typename T, class A = Allocator>
        class AbstractBuffer : public library::Object<A>, public api::Collection<T>, public api::IllegalValue<T>
        {
            typedef library::Object<A> Parent;
    
        public:      
    
            /** 
             * Constructor.
             *
             * @param count count of buffer elements.
             */    
            explicit AbstractBuffer(int32 count) : Parent(),
                count_   (count),
                illegal_ (){
            }
        
            /** 
             * Constructor.
             *
             * @param count   count of buffer elements.
             * @param illegal illegal value.
             */    
            AbstractBuffer(int32 count, const T& illegal) : Parent(),
                count_   (count),
                illegal_ (illegal){
            }
        
            /**
             * Destructor.
             */
            virtual ~AbstractBuffer()
            {
            }
            
            /**
             * Tests if this object has been constructed.
             *
             * @return true if object has been constructed successfully.
             */
            virtual bool isConstructed() const
            {
                return this->isConstructed_;
            }        
                
            /**
             * Fills this buffer by given value.
             *
             * @param value filling value.
             */
            virtual void fill(const T& value)
            {
                fill(value, 0, count_);
            }
            
            /**
             * Fills this buffer by given value.
             *
             * @param value filling value.
             * @param count count of filling elements.
             */
            virtual void fill(const T& value, const int32 count)
            {
                fill(value, 0, count);
            }
        
            /**
             * Fills this buffer by given value.
             *
             * @param value filling value.
             * @param index begin index.
             * @param count count of filling elements.
             */
            virtual void fill(const T& value, const int32 index, const int32 count)
            {
                if( not this->isConstructed_ ) 
                {
                    return;
                }
                if(index >= count_) 
                {
                    return;
                }
                T* buf = getBuffer();
                const int32 max = (index + count <= count_) ? count + index : count_;
                for(int32 i=index; i<max; i++) buf[i] = value;
            }
        
            /**
             * Returns a number of elements.
             *
             * @return number of elements.
             */
            virtual int32 getLength() const
            {
                return count_;
            }
        
            /**
             * Returns this buffer size in byte.
             *
             * @return buffer byte size.
             */
            virtual int32 getSize() const
            {
                return count_ * sizeof(T);
            }
            
            /**
             * Tests if this collection has elements.
             *
             * @return true if this collection does not contain any elements.
             */
            virtual bool isEmpty() const
            {
                return count_ == 0 ? true : false;
            }
        
            /**
             * Returns illegal element which will be returned as error value.
             *
             * If illegal value is not set method returns uninitialized variable.
             *
             * @return reference to illegal element.
             */
            virtual const T& getIllegal() const
            {
                return illegal_;
            }
        
            /**
             * Sets illegal element which will be returned as error value.
             *
             * @param value illegal value.
             */
            virtual void setIllegal(const T& value)
            {
                illegal_ = value;
            }
        
            /**
             * Tests if given value is an illegal.
             *
             * @param value testing value.
             * @param true if value is an illegal.
             */
            virtual bool isIllegal(const T& value) const
            {
                return illegal_ == value ? true : false;
            }
            
            /**
             * Returns an element of this buffer.
             *
             * @param i an element index.
             * @return an element.
             */
            T& operator [](const int32 i)
            {
                T* const buf = getBuffer();
                if( not this->isConstructed_ || i >= count_ || buf == this->NULL) 
                {
                    return illegal_;
                }
                else
                {
                    return buf[i];
                }
            }    
    
        protected:
    
            /**
             * Copies buffer to buffer.
             *
             * If the source buffer greater than this buffer,
             * then only cropped data of that will be and copied.
             *
             * @param buf reference to source buffer.
             */
            virtual void copy(const AbstractBuffer& buf)
            {
                if( not this->isConstructed_ ) 
                {
                    return;
                }
                const int32 size1 = getLength();
                const int32 size2 = buf.getLength();
                const int32 size = size1 < size2 ? size1 : size2;
                T* const buf1 = getBuffer();        
                T* const buf2 = buf.getBuffer();
                for(int32 i=0; i<size; i++) 
                {
                    buf1[i] = buf2[i];
                }
            }
            
            /**
             * Returns a pointer to the fist buffer element.
             *
             * @return pointer to buffer or NULL.
             */
            virtual T* getBuffer() const = 0;
    
        private:
        
            /**
             * Copy constructor.
             *
             * @param obj reference to source object.
             */
            AbstractBuffer(const AbstractBuffer& obj);
            
            /**
             * Assignment operator.
             *
             * @param obj reference to source object.
             * @return reference to this object.     
             */
            AbstractBuffer& operator =(const AbstractBuffer& obj);
            
            /**
             * Number of elements of this buffer.
             */
            int32 count_;
            
            /**
             * Illegal element of this buffer.
             */
            T illegal_;
        
        };
    }
}
#endif // LIBRARY_ABSTRACT_BUFFER_HPP_
