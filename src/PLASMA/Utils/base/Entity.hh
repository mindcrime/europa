#ifndef H_Entity
#define H_Entity

#include "Id.hh"
#include "LabelStr.hh"
#include "PSEntity.hh"

#include <map>
#include <set>
#include <string>
#include <vector>



namespace EUROPA{

#define DECLARE_ENTITY_TYPE(type) \
  static const LabelStr& entityTypeName() { \
    static LabelStr sl_type(#type); \
    return sl_type; \
  } \
  virtual const LabelStr& entityType() { \
    return entityTypeName(); \
  } \

/**
 * @class Entity
 * @brief Basic entity in system
 * @author Conor McGann
 * @ingroup Utility
 */
  class Entity;
  typedef Id<Entity> EntityId;

  // virtual inheritance because we have a diamond (Constraint inherits both Entity and PSConstraint, ie two PSEntities) 
  class Entity: public virtual PSEntity {
  public:
    DECLARE_ENTITY_TYPE(Entity);

    virtual ~Entity();

    inline eint getKey() const {return m_key;}
    inline PSEntityKey getEntityKey() const {return static_cast<PSEntityKey>(cast_int(m_key));}

    
    virtual const std::string& getEntityName() const;
    virtual const std::string& getName() const;

    virtual const std::string& getEntityType() const;

    virtual std::string toString() const;
    virtual std::string toLongString() const;

    
    virtual bool canBeCompared(const EntityId) const;
    /**
     * @brief Get the number of outstanding references
     */
    unsigned int refCount() const;

    /**
     * @brief Increment the reference count. Use if you wish to prevent deletion of the entity
     */
    void incRefCount();

    /**
     * @brief Decrement the reference count. Return true if this self-destructs as a result
     */
    bool decRefCount();

    /**
     * @brief Test of the entity can be deleted. RefCount == 0
     */
    bool canBeDeleted() const;

    /**
     * @brief Retrieve an Entity by key.
     * @return The Id of the requested Entity if present, otherwise a noId;
     */
    static EntityId getEntity(const eint key);

    template<typename T>
    static Id<T> getTypedEntity(const eint key) {return Id<T>(getEntity(key));}
    /**
     * @brief Get all entities
     */
    static void getEntities(std::set<EntityId>& resultSet);


    /**
     * @brief Indicates a system is being terminated
     */
    static void purgeStarted();

    /**
     * @brief Indicates a system is finished terminating
     */
    static void purgeEnded();

    /**
     * @brief Tests if purge in progress
     */
    static bool isPurging();

    /**
     * @brief Test of the entity by the given key is pooled for deallocation
     */
    static bool isPooled(Entity* entity);

  protected:
    Entity();
  private:

    eint m_key;
    
    unsigned int m_refCount;
  };

  /**
   * @brief Key comparator class for ordering in stl containers
   */
  template <class T>
  class EntityComparator{
  public:
    bool operator() (const T& t1, const T& t2) const {
      checkError(t1.isValid(), t1);
      checkError(t2.isValid(), t2);
      return t1->getKey() < t2->getKey();
    }

    bool operator==(const EntityComparator&){return true;}
  };

template<typename T>
class EntityComparator<T*> {
 public:
  bool operator() (const T* t1, const T* t2) const {
    checkError(t1);
    checkError(t2);
    return t1->getKey() < t2->getKey();
  }
};

}
#endif
