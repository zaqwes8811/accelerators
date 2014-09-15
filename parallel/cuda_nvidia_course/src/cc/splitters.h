// http://stackoverflow.com/questions/9985912/how-do-i-choose-grid-and-block-dimensions-for-cuda-kernels
// Можеть стоить еще учитывать warp's?

// Program model: block, grid, warp?
// Hardware model: Stream multyprocessor, warp?

// "Fermi can have up to 48 active warps per SM (1536 threads)"
// "Schedule warps.."

// http://www.cs.berkeley.edu/~volkov/volkov10-GTC.pdf

// http://stackoverflow.com/questions/10460742/how-cuda-blocks-warps-threads-map-onto-cuda-cores?rq=1

// http://www.nvidia.com/content/PDF/fermi_white_papers/NVIDIA_Fermi_Compute_Architecture_Whitepaper.pdf

// http://www.gputechconf.com/gtcnew/on-demand-gtc.php

// For global answers http://docs.nvidia.com/cuda/cuda-c-programming-guide/

// Кажется размер блока лучше брать кратных варпу

#pragma once
#include <vector_types.h>

typedef struct layout2d_s {
  const dim3 block;
  const dim3 grid;
} layout2d_t;

layout2d_t spliGetOpt2DParams(
    const size_t kRows, 
    const size_t kColumns, 
    const size_t kCellSize);
