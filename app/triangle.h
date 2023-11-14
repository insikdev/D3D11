#pragma once
#include <app.h>

class Triangle : public IApp {
public:
    void Init() override;
    void Update() override;
    void Render() override;
};
