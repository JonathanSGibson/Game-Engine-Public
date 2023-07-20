#pragma once
#include "BaseScript.h"
class EnemyScript :
    public BaseScript
{
private:
    bool movingRight;
    float moveSpeed;

    float movementBorderRight; //In global pos
    float movementBorderLeft; //In global pos

    int damage;
public:
    /// Constructor for the enemy script
    /// 
    /// Sets up various variables used in the script
    /// @param owner_ The component's owner
    /// @param moveSpeed_ Sets the enemy's speed
    /// @param movementBorderLeft_ The left side of the walk cycle
    /// @param movementBorderRight_ The right side of the walk cycle
    /// @param damage_ The damage that will be dealt to the plater
    /// @warning This component requires its owner has a Physics component (Make sure to add the physics first!)
    EnemyScript(BaseObject* owner_, float moveSpeed_,  float movementBorderLeft_, float movementBorderRight_, int damage_);

    /// Does all prerender stuff
    /// 
    /// Checks where it is compared to the movement borders and sets forces accordingly
    /// Checks for collisions with the player and damages them
    void Update();

    /// Outputs information to the ImGui window
    /// 
    /// Outputs which direction its moving
    /// Outputs the borders as editable floats
    /// Outputs damage as an editable int
    void DrawGui();
};
