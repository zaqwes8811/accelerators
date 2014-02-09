#include "splitters.h"

// C
#include <math.h>

layout2d_t spliGetOpt2DParams(
    const size_t kRows, 
    const size_t kColumns, 
    const size_t kCellSize) 
  {
  const float kRowToColumns = (1.0f * kRows) / kColumns;
  float yRaw = sqrt(1.0f * kCellSize / kRowToColumns);
  float xRaw = kRowToColumns * yRaw;

  //printf("x = %0.2f y = %0.2f %0.3f\n", xRaw, yRaw, xRaw * yRaw);

  int y = (int)floor(yRaw);
  int x = (int)floor(xRaw);

  // ѕопытка поточнее развести блок
  int rest = kCellSize - x*y;
  int dx = rest/x;
  int dy = rest/y;
  //printf("rest = %d dx = %d dy = %d\n", rest, dx, dy);
  // TODO: может выт€нутьс€ в одном направлении, и это исказить сетку
  /*if (dy > dx) {
    x += dy; 
  } else {
    y += dx;
  }*/

  // TODO: попробовать равномерное расширение.
  /*while () {
  
  }*/

  // TODO: плохо тем, что исправление блока измен€т пропроцию дл€ сетки.
  // TODO:   а значить может стать и хуже

  //printf("BX = %d BY = %d\n", x, y); 

  dim3 blockSize;
  blockSize.x = x;
  blockSize.y = y;
  blockSize.z = 1;

  // »щем размерность сетки
  float xGRowsRaw = (1.0f * kRows) / x;
  float yGRowsRaw = (1.0f * kColumns) / y;

  dim3 gridSize;
  gridSize.x = (int)ceil(xGRowsRaw);
  gridSize.y = (int)ceil(yGRowsRaw);
  gridSize.z = 1;
  const layout2d_t layout = {blockSize, gridSize};
  return layout;
}