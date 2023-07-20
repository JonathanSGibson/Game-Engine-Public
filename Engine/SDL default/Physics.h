#pragma once
#include "BaseComponent.h"

/// Enum for the type of collisions
/// Used when checking for collisions or overlaps
static enum CollisionType
{
    COLLISION,
    OVERLAP
};

///Enum for the 4 sides of a bounding box
/// Used to navigate vectors of floats for the sides of a bounding box
static enum Sides {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};



class Physics :
    public BaseComponent
{
private:
     Vector2f topLeft;
     Vector2f topRight;
     Vector2f bottomRight;
     Vector2f bottomLeft;

     Vector2f centre;

     float top;
     float left;
     float bottom;
     float right;

     CollisionType colliderType;

     bool dynamic;
     bool hasGravity;


     Vector2f forces;

     const float gravity = 20.0f;

     const float bufferDistance = 0.0f;
public:
    /// Constructor for the physics component
    /// 
    /// Sets various required variables of the component based on the parameters
    /// Calls the UpdateCorners() function to initially set the collision box
    /// @param owner_ The owner of the component
    /// @param dynamic_ Sets whether the object is dynamic (can move) or not, defaulted to false
    /// @param dynamic_ Sets whether the object is effected by gravity or not, defaulted to false
    /// @param collisionType_ Sets the type of collision of the object as an enum
    /// @warning This component requires the owner has a sprite component (Make sure to add the sprite first!)
    Physics(BaseObject* owner_, bool dynamic_ = false , bool gravity_ = false, CollisionType collisionType_ = COLLISION);

    /// Updates the bounds of the collision box
    /// 
    /// Uses the owner's sprite component to get the location, width and height of the sprite and set the collision box accordingly
    void UpdateCorners();

    /// Returns the corners of the collision box
    /// 
    /// Formatted as top left, top right, bottom right, bottom left
    /// There is an enum included in the Physics header which can be used to navigate this instead of manually inputting integers
    /// @returns A vector of Vector2fs which each contain 2 floats, each Vector2f is the screen position of one corner
    vector<Vector2f> GetCorners();

    /// Returns the sides of the collision box
    /// 
    /// Gets the sides of the collision box
    /// @returns A vector of floats which represent either the X (in the case of left and right) or Y (in the case of top and bottom) position of the collision box
    vector<float> GetSides();

    /// Gets the centre of the collision box
    /// 
    ///@returns Vector2f containing the coordinates of the centre of the collision box
    Vector2f GetCentre() { return centre; }

    /// Gets the forces of the component
    /// 
    /// @returns Vector2f containing both the horizontal and vertical forces being applied to the component
    /// @note As currently there is no friction or air resistance these forces can also functionally be used as the velocity of component
    Vector2f GetForces() {return forces;}

    /// Gets the current overlaps with the collision box
    /// 
    /// Finds and returns any objects which have a physics component with the OVERLAP CollisionType and overlap with this objects collision box
    /// @param tag Only overlaps of objects with the given tag are returned, if the given tag is an empty string (which it defaults to) it will return ALL overlaps
    /// @returns A vector of BaseObject pointers which contains every object which overlaps, has the given tags and has the OVERLAP CollisionType
    vector<BaseObject*> GetOverlaps(string tag = "");

    /// Gets the current collisions with the collision box
    /// 
    /// Finds and returns any objects which have a physics component with the COLLISION CollisionType and overlap with this objects collision box
    /// @param tag Only collisions of objects with the given tag are returned, if the given tag is an empty string (which it defaults to) it will return ALL overlaps
    /// @returns A vector of BaseObject pointers which contains every object which collides with this object, has the given tags and has the COLLIDER CollisionType
    vector<BaseObject*> GetCollisions(string tag = "");

    /// Gets the CollisionType of the component
    /// @returns The CollisionType of the component
    CollisionType GetCollisionType() { return colliderType; }

    /// Sets the CollisionType of the component
    /// 
    /// @param newType The CollisionType, the type is an enum in the Physics header
    void SetCollisionType(CollisionType newType) { colliderType = newType; }

    /// Gets the bool of whether the component is dynamic or not
    /// 
    /// Gets whether the object is dynamic (can move) or not
    /// @returns A bool of whether the object is dynamic
    bool GetDynamic() { return dynamic; }

    /// Gets the bool of whether the component has gravity
    /// 
    /// Gets whether the object is dynamic (can move) or not
    /// @returns A bool of whether the object is effected by gravity or not
    bool GetGravity() { return gravity; }

    /// Adds various information to the ImGui window
    /// 
    /// Outputs the following information to the ImGui window:
    ///     - The ColliderType
    ///     - A checkbox to show and set the dynamic bool
    ///     - A checkbox to show and set the gravity bool
    ///     - The forces applying to the component
    void DrawGui();

    /// Runs all the code which needs to be run every frame
    /// 
    /// Checks if the object has gravity, if so adds gravity to the forces
    /// Checks if the object is dynamic, if so moves the object and checks for collisions with objects with the "Platform" tag
    void Update();

    /// Adds a force to the existing forces
    /// 
    /// @param newForce The new force to be added to the existing forces
    /// @warning AddForce(Vector2f newForce) and SetForce(Vector2f newForce) are distinct and should not be mixed up
    void AddForce(Vector2f newForce);

    /// Sets the forces applying to the object
    /// 
    /// @param newForce The force that will replace any current forces applying to the component
    /// @warning SetForce(Vector2f newForce) and AddForce(Vector2f newForce) are distinct and should not be mixed up
    void SetForce(Vector2f newForce) { forces = newForce; }

    /// Sets the horizontal force
    /// 
    /// As SetForce but only setting the force in X
    /// @param newForce The force that will replace the current X force
    void SetXForce(float newForce) { forces.x = newForce; }

    /// Sets the vertical force
    /// 
    /// As SetForce but only setting the force in Y
    /// @param newForce The force that will replace the current Y force
    /// @note Y is measured from the top left of the window, so a positive force in Y is a force pushing down
    void SetYForce(float newForce) { forces.y = newForce; }

    /// Sets the objects position by repositioning its top
    /// 
    /// Sets the objects position by repositioning its top
    /// Used to reposition when collisions occur
    /// @param value The new value of top to calculate the position of the object from
    void SetTop(float value);

    /// Sets the objects position by repositioning its right
    /// 
    /// Sets the objects position by repositioning its right
    /// Used to reposition when collisions occur
    /// @param value The new value of right to calculate the position of the object from
    void SetRight(float value);

    /// Sets the objects position by repositioning its bottom
    /// 
    /// Sets the objects position by repositioning its bottom
    /// Used to reposition when collisions occur
    /// @param value The new value of bottom to calculate the position of the object from
    void SetBottom(float value);

    /// Sets the objects position by repositioning its left
    /// 
    /// Sets the objects position by repositioning its left
    /// Used to reposition when collisions occur
    /// @param value The new value of left to calculate the position of the object from
    void SetLeft(float value);
};

