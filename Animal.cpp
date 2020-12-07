#include "Animal.hpp"
#include "River.hpp"
#include <iostream>
#include "LitToxicColorTextureProgram.hpp"


// Load any drawables
void Animal::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Animal_Dead"));
  this->dead = &(scene->drawables.back());
  this->extra_drawables.push_back(this->dead);
  this->dead->disabled = true;

}


// Create a copy with no drawables
Animal* Animal::clone_lightweight(Cell* new_cell) {
  Animal* new_animal = new Animal(*this);
  new_animal->cell = new_cell;
  new_animal->drawable = nullptr;
  new_animal->dead = nullptr;
  new_animal->extra_drawables.clear();
  return new_animal;
}

void Animal::on_post_tick(){
	if(!is_dead && dynamic_cast<River*>(this->cell->bgTile) != nullptr) {
    River *r = dynamic_cast<River*>(this->cell->bgTile); 
    if(r->is_contaminated > 0 && r->is_contaminated < r->max_contaminated) {
      this->drawable->pipeline.set_uniforms = []() {
        glUniform1f(lit_toxic_color_texture_program->PURPLE_AMT_float, 0.2f);
      };
    }
    if(r->is_contaminated == r->max_contaminated) {
      is_dead = true; 
      this->dead->disabled = false; 
      this->drawable->disabled = true; 
            this->dead->pipeline.set_uniforms = []() {
        glUniform1f(lit_toxic_color_texture_program->PURPLE_AMT_float, 0.4f);
      };
      AudioManager::clips_to_play.push(AudioManager::AudioClip::KILL_DUCK);

      //TODO: balance this? 
      current_grid->grid_environment_score -= 10;

    }
  }
}
