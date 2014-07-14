/*
    This file is part of darktable,
    copyright (c) 2014 LebedevRI.

    darktable is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    darktable is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with darktable.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdint.h>
#include <xmmintrin.h>

#include "develop/imageop.h"
#include "develop/pixelpipe.h"
#include "common/image.h"
#include "develop/tiling.h"

DT_MODULE(1)

typedef struct dt_iop_letsgofloat_params_t
{
  int keep;
}
dt_iop_letsgofloat_params_t;

const char *name()
{
  return C_("modulename", "let's go float!");
}

int
flags()
{
  return IOP_FLAGS_ALLOW_TILING | IOP_FLAGS_ONE_INSTANCE |
         IOP_FLAGS_HIDDEN | IOP_FLAGS_NO_HISTORY_STACK;
}

int
groups()
{
  return IOP_GROUP_BASIC;
}

int
output_bpp(
  dt_iop_module_t *self,
  dt_dev_pixelpipe_t *pipe,
  dt_dev_pixelpipe_iop_t *piece)
{
  return sizeof(float);
}

void
process(
  struct dt_iop_module_t *self,
  dt_dev_pixelpipe_iop_t *piece,
  const void *const ivoid,
  void *ovoid,
  const dt_iop_roi_t *const roi_in,
  const dt_iop_roi_t *const roi_out)
{
  const  float divider  = (float)UINT16_MAX;
  const __m128 dividers = _mm_set_ps1(divider);

#ifdef _OPENMP
  #pragma omp parallel for default(none) schedule(static) shared(ovoid)
#endif
  for(int j = 0; j < roi_out->height; j++)
  {
    const uint16_t *in = ((uint16_t *)ivoid) + (size_t)j * roi_out->width;
    float *out = ((float *)ovoid) + (size_t)j * roi_out->width;

    int i = 0;
    int alignment = ((8 - (j * roi_out->width & (8 - 1))) & (8 - 1));

    // process unaligned pixels
    for ( ; i < alignment ; i++, out++, in++)
      *out = ((float)(*in)) / divider;

    // process aligned pixels with SSE
    for( ; i < roi_out->width - (8 - 1); i += 8, in += 8)
    {
      const __m128i input = _mm_load_si128((__m128i *)in);

      __m128i ilo = _mm_unpacklo_epi16(input, _mm_set1_epi16(0));
      __m128i ihi = _mm_unpackhi_epi16(input, _mm_set1_epi16(0));

      __m128 flo = _mm_cvtepi32_ps(ilo);
      __m128 fhi = _mm_cvtepi32_ps(ihi);

      flo = _mm_div_ps(flo, dividers);
      fhi = _mm_div_ps(fhi, dividers);

      _mm_stream_ps(out, flo);
      out += 4;
      _mm_stream_ps(out, fhi);
      out += 4;
    }

    // process the rest
    for( ; i < roi_out->width; i++, out++, in++)
      *out = ((float)(*in)) / divider;
  }
  _mm_sfence();
}

void
commit_params(
  struct dt_iop_module_t *self,
  dt_iop_params_t *params,
  dt_dev_pixelpipe_t *pipe,
  dt_dev_pixelpipe_iop_t *piece)
{
  if(!(pipe->image.flags & DT_IMAGE_RAW) ||
      dt_dev_pixelpipe_uses_downsampled_input(pipe))
    piece->enabled = 0;
}

void
init(
  dt_iop_module_t *self)
{
  self->params = calloc(1, sizeof(dt_iop_letsgofloat_params_t));
  self->default_params = calloc(1, sizeof(dt_iop_letsgofloat_params_t));
  self->hide_enable_button = 1;
  self->default_enabled = 1;
  self->priority = 5; // module order created by iop_dependencies.py, do not edit!
  self->params_size = sizeof(dt_iop_letsgofloat_params_t);
  self->gui_data = NULL;
}

void
cleanup(
  dt_iop_module_t *self)
{
  free(self->params);
  self->params = NULL;
}

// modelines: These editor modelines have been set for all relevant files by tools/update_modelines.sh
// vim: shiftwidth=2 expandtab tabstop=2 cindent
// kate: tab-indents: off; indent-width 2; replace-tabs on; indent-mode cstyle; remove-trailing-space on;
