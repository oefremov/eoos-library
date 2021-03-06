/**
 * Abstract class for sequential accessing to data store.
 *
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016, Sergey Baigudin, Baigudin Software
 * @license   http://embedded.team/license/
 */
#ifndef LIBRARY_ABSTRACT_LINKED_LIST_HPP_
#define LIBRARY_ABSTRACT_LINKED_LIST_HPP_

#include "library.Object.hpp"
#include "library.Buffer.hpp"
#include "library.LinkedNode.hpp"
#include "api.List.hpp"
#include "api.Queue.hpp"
#include "api.Iterable.hpp"

namespace local
{
    namespace library
    {
        /**
         * Primary template implementation.
         *
         * @param T data type of container element.
         * @param A heap memory allocator class.
         */
        template <typename T, class A = Allocator>
        class AbstractLinkedList :
            public library::Object<A>,
            public api::List<T>,
            public api::Queue<T>,
            public api::Iterable<T>{

            typedef library::AbstractLinkedList<T,A> Self;
            typedef library::Object<A>               Parent;
            typedef library::LinkedNode<T,A>         Node;

        public:

            /**
             * Constructor.
             */
            AbstractLinkedList() : Parent(),
                illegal_ (),
                last_    (NULL),
                count_   (0){
            }

            /**
             * Constructor.
             *
             * NOTE: A passed element must be copied to an internal data structure of
             * this class by calling a copy constructor so that the element
             * might be invalidated after the function called.
             *
             * @param illegal - an illegal element.
             */
            AbstractLinkedList(const T& illegal) : Parent(),
                illegal_ (illegal),
                last_    (NULL),
                count_   (0){
            }

            /**
             * Destructor.
             */
            virtual ~AbstractLinkedList()
            {
                clear();
            }

            /**
             * Tests if this object has been constructed.
             *
             * @return true if object has been constructed successfully.
             */
            virtual bool isConstructed() const
            {
                return Parent::isConstructed();
            }

            /**
             * Inserts new element to the end of this list.
             *
             * @param element inserting element.
             * @return true if element is added.
             */
            virtual bool add(const T& element)
            {
                return Self::isConstructed() ? addNode(getLength(), element) : false;
            }

            /**
             * Inserts new element to the specified position in this list.
             *
             * @param index   position in this list.
             * @param element inserting element.
             * @return true if element is inserted.
             */
            virtual bool add(int32 const index, const T& element)
            {
                return Self::isConstructed() ? addNode(index, element) : false;
            }

            /**
             * Removes all elements from this list.
             */
            virtual void clear()
            {
                if( not Self::isConstructed() )
                {
                    return;
                }
                const int32 b = getLength() - 1;
                for(int32 i=b; i>=0; i--)
                {
                    if( not removeNode( getNodeByIndex(i) ) )
                    {
                        break;
                    }
                }
            }

            /**
             * Removes the first element from this list.
             *
             * @return true if an element is removed successfully.
             */
            virtual bool removeFirst()
            {
                return remove(0);
            }

            /**
             * Removes the last element from this list.
             *
             * @return true if an element is removed successfully.
             */
            virtual bool removeLast()
            {
                return remove( getLength() - 1 );
            }

            /**
             * Removes the head element of this queue or list.
             *
             * @return true if an element is removed successfully.
             */
            virtual bool remove()
            {
                return remove(0);
            }

            /**
             * Removes the element at the specified position in this list.
             *
             * @param index   position in this list.
             * @return true if an element is removed successfully.
             */
            virtual bool remove(const int32 index)
            {
                return Self::isConstructed() ? removeNode( getNodeByIndex(index) ) : false;
            }

            /**
             * Removes the first occurrence of the specified element from this list.
             *
             * @param element reference to element.
             * @return true if an element is removed successfully.
             */
            virtual bool removeElement(const T& element)
            {
                return Self::isConstructed() ? removeNode( getNodeByElement(element) ) : false;
            }

            /**
             * Examines the head element of this container.
             *
             * @return the head element.
             */
            virtual T& peek() const
            {
                return get(0);
            }

            /**
             * Returns the first element in this container.
             *
             * @return the first element in this container.
             */
            virtual T& getFirst() const
            {
                return get(0);
            }

            /**
             * Returns the last element in this container.
             *
             * @return the last element in this container.
             */
            virtual T& getLast() const
            {
                return get( getLength() - 1 );
            }

            /**
             * Returns an element from this container by index.
             *
             * @param index - position in this container.
             * @return indexed element of this container.
             */
            virtual T& get(int32 index) const
            {
                if( not Self::isConstructed() )
                {
                    return illegal_;
                }
                Node* const node = getNodeByIndex(index);
                return node != NULL ? node->getElement() : illegal_;
            }

            /**
             * Returns a number of elements in this list.
             *
             * @return number of elements.
             */
            virtual int32 getLength() const
            {
                return last_ == NULL ? 0 : last_->getIndex() + 1;
            }

            /**
             * Tests if this list has elements.
             *
             * @return true if this list does not contain any elements.
             */
            virtual bool isEmpty() const
            {
                return last_ == NULL ? true : false;
            }

            /**
             * Returns illegal element which will be returned as error value.
             *
             * If illegal value is not set method returns uninitialized variable.
             *
             * @return illegal element.
             */
            virtual T& getIllegal() const
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
                if( Self::isConstructed() )
                {
                    illegal_ = value;
                }
            }

            /**
             * Tests if given value is an illegal.
             *
             * @param value testing value.
             * @param true if value is an illegal.
             */
            virtual bool isIllegal(const T& value) const
            {
                if( not Self::isConstructed() )
                {
                    return false;
                }
                return illegal_ == value ? true : false;
            }

            /**
             * Returns the index of the first occurrence of the specified element in this list.
             *
             * @param element reference to the element.
             * @return index or -1 if this list does not contain the element.
             */
            virtual int32 getIndexOf(const T& element) const
            {
                Node* const node = getNodeByElement(element);
                return node != NULL ? node->getIndex() : -1;
            }

            /**
             * Tests if given index is available.
             *
             * @param index checking position in this list.
             * @return true if index is present.
             */
            virtual bool isIndex(int32 const index) const
            {
                return (0 <= index && index < getLength()) ? true : false;
            }

            /**
             * Returns an array of all list links to elements.
             *
             * You have to call delete operator for returned value after it have used.
             *
             * @return pointer to reference of elements or NULL if list is empty.
             */
            virtual library::Buffer<T,0,A>* array() const
            {
                #ifdef EOOS_NO_STRICT_MISRA_RULES
                if( not Self::isConstructed() )
                {
                    return NULL;
                }
                const int32 count = getLength();
                if(count == 0)
                {
                    return NULL;
                }
                Buffer<T,0,A>* buf = new Buffer<T,0,A>(count, illegal_);
                if(buf == NULL || not buf->isConstructed())
                {
                    delete buf;
                    return NULL;
                }
                Node* node = last_->getNext();
                for(int32 i=0; i<count; i++)
                {
                    (*buf)[i] = node->getElement();
                    node = node->getNext();
                }
                return buf;
                #else
                return NULL;
                #endif
            }

            /**
             * Returns an iterator of this list elements.
             *
             * @return pointer to new itererator.
             */
            virtual api::Iterator<T>* getIterator()
            {
                return this->getListIterator(0);
            }

        protected:

            /**
             * Inserts new element to the specified position in this list.
             *
             * Given element will be copied to self nodes structure by a copy constructor calling.
             *
             * @param index   position in this list.
             * @param element inserting element.
             * @return true if element is inserted.
             */
            bool addNode(const int32 index, const T& element)
            {
                if(isIndexOutOfBounds(index))
                {
                    return false;
                }
                Node* const node = new Node(element);
                if(node == NULL || not node->isConstructed())
                {
                    delete node;
                    return false;
                }
                if(last_ == NULL)
                {
                    last_ = node;
                    count_++;
                    return true;
                }
                if(index > 0)
                {
                    Node* const after = getNodeByIndex(index - 1);
                    if(after == NULL)
                    {
                        delete node;
                        return false;
                    }
                    after->insertAfter(node);
                    if(after == last_)
                    {
                        last_ = node;
                    }
                }
                else
                {
                    Node* const before = getNodeByIndex(0);
                    if(before == NULL)
                    {
                        delete node;
                        return false;
                    }
                    before->insertBefore(node);
                }
                count_++;
                return true;
            }

            /**
             * Returns a node of this list by index.
             *
             * @param index position in this list.
             * @return pointer to the node of this list.
             */
            Node* getNodeByIndex(const int32 index) const
            {
                if( not isIndex(index) )
                {
                    return NULL;
                }
                if(index == getLength() - 1)
                {
                    return last_;
                }
                Node* node = last_->getNext();
                for(int32 i=0; i<index; i++)
                {
                    node = node->getNext();
                }
                return node;
            }

            /**
             * Returns a node of this list by element.
             *
             * @param element reference to element.
             * @return pointer to the node of this list.
             */
            Node* getNodeByElement(const T& element) const
            {
                const int32 len = getLength();
                if(len == 0)
                {
                    return NULL;
                }
                Node* node = last_->getNext();
                for(int32 i=0; i<len; i++, node = node->getNext())
                {
                    if(element != node->getElement())
                    {
                        continue;
                    }
                    return node;
                }
                return NULL;
            }

            /**
             * Removes a node of this list.
             *
             * @param node pointer to node.
             * @return true if a node is removed successfully.
             */
            bool removeNode(Node* const node)
            {
                if(node == NULL)
                {
                    return false;
                }
                if(node == last_)
                {
                    if(getLength() == 1)
                    {
                        last_ = NULL;
                    }
                    else
                    {
                        last_ = last_->getPrevious();
                    }
                }
                delete node;
                count_++;
                return true;
            }

            /**
             * Tests if index is out of this list bounds.
             *
             * @param index checking position in this list.
             * @return true if index is outed.
             */
            bool isIndexOutOfBounds(const int32 index) const
            {
                return (index < 0 || index > getLength()) ? true : false;
            }

            /**
             * Returns reference to self data value.
             *
             * @return data value.
             */
            int32& getReferenceToCount()
            {
                return count_;
            }

            /**
             * Returns reference to self data value.
             *
             * @return data value.
             */
            Node*& getReferenceToLast()
            {
                return last_;
            }

            /**
             * Returns reference to self data value.
             *
             * @return data value.
             */
            T& getReferenceToIllegal()
            {
                return illegal_;
            }

        private:

            /**
             * Copy constructor.
             *
             * @param obj reference to source object.
             */
            AbstractLinkedList(const AbstractLinkedList& obj);

            /**
             * Assignment operator.
             *
             * @param obj reference to source object.
             * @return reference to this object.
             */
            AbstractLinkedList& operator=(const AbstractLinkedList& obj);

            /**
             * Illegal element of this list.
             */
            mutable T illegal_;

            /**
             * Last node of this list.
             */
            Node* last_;

            /**
             * Number of changes in this list.
             */
            int32 count_;

        };
    }
}
#endif // LIBRARY_ABSTRACT_LINKED_LIST_HPP_
