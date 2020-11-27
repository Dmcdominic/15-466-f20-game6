#include "Cloud.hpp"
#include <iostream>


// No need to override anything. Pushable is default FgObj behavior.

Cloud::Cloud(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Cloud")); 
  this->drawable = &(scene->drawables.back());
}
