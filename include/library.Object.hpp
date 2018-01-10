/** 
 * Root class of the library class hierarchy.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2018, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_OBJECT_HPP_
#define LIBRARY_OBJECT_HPP_

#include "Object.hpp"
#include "library.Allocator.hpp"

namespace library
{
    template <class Alloc = Allocator>    
    class Object : public ::Object<Alloc>
    {
        typedef ::Object<Alloc> Parent;        
      
    public:
    
        /** 
         * Constructor.
         */  
        Object() : Parent(),
            isConstructed_ (this->getConstruct()){
        }
        
        /** 
         * Copy constructor.
         *
         * @param obj reference to source object.
         */ 
        Object(const Object& obj) : Parent(obj),
            isConstructed_ (this->getConstruct()){
        }
        
        /** 
         * Copy constructor.
         *
         * @param obj reference to source object.
         */ 
        Object(const ::api::Object& obj) : Parent(obj),
            isConstructed_ (this->getConstruct()){
        }    
        
        /** 
         * Destructor.
         */    
        virtual ~Object()
        {
        }  
        
        /**
         * Tests if this object has been constructed.
         *
         * @return true if object has been constructed successfully.
         */    
        virtual bool isConstructed() const
        {
            return isConstructed_;
        }
        
        /** 
         * Assignment operator.
         *
         * @param obj reference to source object.
         * @return reference to this object.   
         */  
        Object& operator =(const Object& obj)
        {
            Parent::operator=(obj);
            return *this;
        }        
   
    protected:
        
        /** 
         * The root object constructed flag.
         */  
        const bool& isConstructed_;
    
    };
}
#endif // LIBRARY_OBJECT_HPP_