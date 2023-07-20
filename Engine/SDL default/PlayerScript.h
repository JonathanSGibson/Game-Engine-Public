#pragma once
#include "BaseScript.h"
#include <ctime>
class PlayerScript :
    public BaseScript
{
private:
    //Player variables
    int health;
    int maxHealth;
    float moveSpeed;
    float jumpForce;
    float invulnerableDuration;
    clock_t invulnerableStart;
    bool hasKey; // Arguable stuff to do with pickups should be in a seperate script, however for the sake of simplicity for this project I will be keeping it in this one
    bool canJump;
    bool moveRight;
    bool moveLeft;
    bool invulnerable; //Set when you get hit
    Vector2f respawnPoint;


public:
    /// Constructor for the player script
    /// 
    /// Adds this as a listener for the left, right and jump events
    /// @param owner_ The component's owner
    /// @param maxHealth_ The max health of the player
    /// @param moveSpeed_ The speed of the player
    /// @param jumpForce How much force is applied when the player jumps
    /// @param invulnerableDuration_ The duration of iframes after the player is hit
    /// @param respawnPoint_ The point the player is moved to when they die
    /// @warning This component requires its owner has a Physics component (Make sure to add the physics first!)
    PlayerScript(BaseObject* owner_, int maxHealth_, float moveSpeed_, float jumpForce_, float invulnerableDuration_, Vector2f respawnPoint_);

    /// Damages the player
    /// 
    /// Checks if the player has died and if so call Respawn() if not (and the damage is positive) starts the players iframes
    /// @param damage The amount of damage to deal to the player
    /// @note to heal the player simply call this with a negative value
    void DealDamage(int damage); //If healing is requires you can simply deal negative damage

    /// Respawns the player
    /// 
    /// Sets the health to max and moves the player to the respawn point
    void Respawn();

    /// Adds a vertical force to the player
    void Jump();

    /// Sets the player's respawn point
    /// 
    /// @param checkpointPosition The new respawn point
    void Checkpoint(Vector2f checkpointPosition);

    /// Handles events
    /// 
    /// If the event is jump it calls Jump()
    /// If the event is left or right it checks whether to start or stop the player's movement
    /// @param currentEvent The event to be handled
    void HandleEvent(BaseEvent* currentEvent);

    /// Outputs information to the ImGui window
    /// 
    /// Outputs the health
    /// Outputs the max health as an editable int
    /// Outputs the move speed as an editable float
    /// Outputs the jump force as an editable float
    /// Outputs the invulnerability dutation as an editable float
    /// Outputs whether the player can jump or not
    /// Outputs whether the player is invulnerable or not
    /// Outputs the respawn position as 2 editable floats
    void DrawGui();

    /// All functions to run every frame
    /// 
    /// Shows the health bar utilising ImGui
    /// Checks if invulnerablity is over
    /// Checks which way the player is moving and applies appropriate forces
    /// Calls CheckForOverlaps()
    void Update();

    /// Checks for overlaps
    /// 
    /// Checks for overlaps
    /// If it overlaps with a key sets key to true
    /// If it overlaps with a door and key is true runs EndLevel()
    void CheckForOverlaps();

    /// Sets the currently active scene to the next scene
    void EndLevel();

    /// Returns if the player is invulnerable or not
    bool GetInvulnerable() { return invulnerable; }
};

