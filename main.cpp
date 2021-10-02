#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>

/*#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif*/

typedef enum {
  NONE, UP, DOWN, LEFT, RIGHT
} key_input;

typedef struct {
  SDL_Renderer *renderer;
  SDL_Rect dest;
  SDL_Texture *pl_tex;

  key_input key_state;
  int posx;
  int posy;
} ctxt;


int test_image(ctxt *ctx) {
  SDL_Surface *image = IMG_Load("resources/player.png");
  if(!image) { printf("IMG_Load: %s\n",IMG_GetError());
    return 0; }
  //int result = image->w;

  ctx->pl_tex = SDL_CreateTextureFromSurface(ctx->renderer,image);
  ctx->dest.w = image->w;
  ctx->dest.h = image->h;
  SDL_FreeSurface(image);

  /*SDL_Rect dest = {.x = 200, .y = 100, .w = 200, .h = 200};
  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer,image);
  SDL_RenderCopy(renderer,tex,NULL,&dest);
  SDL_DestroyTexture(tex);
  SDL_FreeSurface(image);
  return result;*/
  return 1;
}

void proc_input(ctxt *ctx) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.key.keysym.sym) {
      case SDLK_UP: {
	if(event.key.type == SDL_KEYDOWN) { ctx->key_state = UP; }
	else if (event.key.type == SDL_KEYUP) { ctx->key_state = NONE; }
	break; }
      case SDLK_DOWN: {
	if(event.key.type == SDL_KEYDOWN) { ctx->key_state = DOWN; }
	else if (event.key.type == SDL_KEYUP) { ctx->key_state = NONE; }
	break; }
      case SDLK_LEFT: {
	if(event.key.type == SDL_KEYDOWN) { ctx->key_state = LEFT; }
	else if (event.key.type == SDL_KEYUP) { ctx->key_state = NONE; }
	break; }
      case SDLK_RIGHT: {
	if(event.key.type == SDL_KEYDOWN) { ctx->key_state = RIGHT; }
	else if (event.key.type == SDL_KEYUP) { ctx->key_state = NONE; }
	break; }
      default: break;
    }
  }
}

void loop_handle(void *arg) {
  ctxt *ctx = (ctxt *)arg;
  proc_input(ctx);
  switch(ctx->key_state) {
    case NONE: break;
    case UP: { ctx->dest.y += 0.01; break; }
    case DOWN: { ctx->dest.y -= 0.01; break; }
    case LEFT: { ctx->dest.x -= 0.01; break; }
    case RIGHT: { ctx->dest.x += 0.01; break; } 
    default: break; }
  SDL_RenderClear(ctx->renderer);
  SDL_RenderCopy(ctx->renderer,ctx->pl_tex,NULL,&ctx->dest);
  SDL_RenderPresent(ctx->renderer);
}

int main(int argc, char **argv) {
  //printf("Hello world\n");

  SDL_Window *window = NULL;
  //SDL_Renderer *renderer = NULL;
  ctxt ctx;
  ctx.key_state = NONE;
  ctx.dest.x = 200;
  ctx.dest.y = 100;
  ctx.posx = 200;
  ctx.posy = 100;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized! SDL_Error: %s\n",SDL_GetError()); }
  else {
    SDL_CreateWindowAndRenderer(600,400,0,&window,&ctx.renderer);
    int result = 0;
    SDL_SetRenderDrawColor(ctx.renderer,255,255,255,255);
    test_image(&ctx);
    /*SDL_RenderClear(renderer);
    result |= test_image(renderer,"resources/player.png");
    SDL_RenderPresent(renderer);*/ }
  emscripten_set_main_loop_arg(loop_handle,&ctx,-1,1);
  return 0;
}
