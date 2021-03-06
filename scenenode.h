#ifndef SCENENODE_H
#define SCENENODE_H

#include <Eigen/Geometry>
#include <memory>
#include <string>
#include <list>
#include "component.h"
#include <vector>

class SceneNode
{
friend class Scene;
private:
    SceneNode* parent_;

    std::list<std::unique_ptr<SceneNode> > children_;
    std::list<std::unique_ptr<Component> > components_;
    std::string name_;

    Eigen::Quaternion<float> rotation_;
    Eigen::Vector3f translation_;
    Eigen::Vector3f scale_;

    bool components_sorted_;
    bool marked_for_delete_;

private:
    void checkForDeletions();
    //forwards SceneNode, its components, and its children by a frame
    void frame(const Eigen::Affine3f& parent_world_transform);


public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    SceneNode();
    SceneNode(std::string name);
    SceneNode(const SceneNode& other);
    SceneNode& operator = (const SceneNode& other);
    ~SceneNode();

    /**
     * @brief Gets name of the SceneNode
     * @return a string containing the name
     */
    std::string name();

    /**
     * @brief Sets name of SceneNode to /p nme
     * @param name name to set SceneNode to
     */
    void name(std::string nme);

    /**
     * @brief Calculates world transform of the SceneNode
     * @return world transform as an Eigen::Affine3f
     */
    Eigen::Affine3f worldTransform();

    /**
     * @brief Gets local rotation of the SceneNode
     * @return an Eigen::Quaternion depicting the local rotation
     */
    Eigen::Quaternion<float> rotation();

    /**
     * @brief Gets local translation of the SceneNode
     * @return an Eigen::Vector3f depicting the local translation
     */
    Eigen::Vector3f translation();

    /**
     * @brief Gets scale of the SceneNode
     * @return an Eigen::Vector3f depicting the scale
     */
    Eigen::Vector3f scale();

    /**
     * @brief Sets rotation of SceneNode to /p rot
     * @param rot The rotation to be set
     */
    void rotation(const Eigen::Quaternion<float>& rot);

    /**
     * @brief Rotates the SceneNode by /p rot
     * @param rot The rotation to add
     */
    void rotateBy(const Eigen::Quaternion<float>& rot);

    /**
     * @brief Sets translation of SceneNode to /p trans
     * @param trans The translation to be set
     */
    void translation(const Eigen::Vector3f& trans);

    /**
     * @brief Sets the scale of the SceneNode to /p scl
     * @param scl The scale to be set
     */
    void scale(const Eigen::Vector3f& scl);

    /**
     * @brief Gets world rotation of the SceneNode
     * @return the world rotation of the SceneNode as an Eigen::Quaternion
     */
    Eigen::Quaternion<float> worldRotation();

    /**
     * @brief Gets the world translation of the SceneNode
     * @return the world translation of the SceneNode as an Eigen::Vector3f
     */
    Eigen::Vector3f worldTranslation();

    /**
     * @brief Finds the first occurence of a child with the specified /p name. All descendants are searched, using depth first
     * @param name Name of the child to find
     * @return an observer pointer to the first descedant with the given name
     */
    SceneNode* findChild(const std::string& name);

    /**
     * @brief Finds all descendants with the given /p name. Search is performed depth first, with the children being in that order.
     * @param name Name of the children to find
     * @return a list of observer pointers to all the children with the given name
     */
    std::list<SceneNode*> findChildren(const std::string& name);

    /**
     * @brief Searches descendants for the passed \p child
     * @param child Child to search for
     * @return true if \p child was found, else false
     */
    bool findChildByPointer(const SceneNode* child);

    /**
     * @brief Adds /p component to the SceneNode
     * @param component Component to be added
     */
    void addComponent(std::unique_ptr<Component>&& component);

    /**
     * @brief Removes component /p component from the SceneNode
     * @param component Component to be removed
     */
    void removeComponent(Component* component);

    /**
     * @brief Gets first component of ComponentType
     */
    template<typename ComponentType>
    Component* getComponent();

    /**
     * @brief Gets all components of ComponentType
     */
    template<typename ComponentType>
    std::vector<Component*> getComponents();

    /**
     * @brief Adds the specified child to the SceneNode
     * @param child Child node to be added
     */
    void addChild(std::unique_ptr<SceneNode>&& child);

    /**
     * @brief Creates and adds a child of the given name to the current SceneNode
     * @return an observer pointer to the added child
     */
    SceneNode* addChild(std::string name = "Nameless");

    /**
     * @brief Removes the child with the given pointer. Instead of deleting the child, the unique pointer containing the child is transfered to the caller.
     * This allows the shifting around of SceneNodes without the need for copying.
     * @param child Pointer to child to be removed
     * @return unique_ptr containing the removed child
     */
    std::unique_ptr<SceneNode> removeChild(const SceneNode* child);

    /**
     * @brief Gets the root node of the scene that the current node belongs to
     * @return observer pointer to the root of the current scene node
     */
    SceneNode* getRoot();

    /**
     * @brief Marks scene node for destruction, which will occur in the next frame
     */
    void destroy();
};

#endif // SCENENODE_H
