#include "entities.hpp"

void Player::setup(int x, int y, int z) {
    this->_cam.position.x = x;
    this->_cam.position.y = y;
    this->_cam.position.z = z;
    this->_renderTexture = LoadRenderTexture(playerTextureWidth, playerTextureHeight);
}
void Player::update() {

    UpdateCamera(&_cam, CAMERA_FIRST_PERSON);
    if (IsKeyDown(KEY_SPACE)) {
        this->_cam.target.y += 1;
        this->_cam.position.y += 1;
    }
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        this->_cam.target.y -= 1;
        this->_cam.position.y -= 1;
        std::cout << "a";
    }
    this->_direction = Vector3Subtract(this->_cam.position, this->_cam.target);
    std::cout << this->_direction.x << " " << this->_direction.y << " " << this->_direction.z << "\n";
    Vector3 _cube = { _cam.position.x,-4,_cam.position.z };
    DrawCube(_cube, 8, 8, 8, RED);
}