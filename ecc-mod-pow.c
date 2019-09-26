/* ecc-mod-pow.c

   Copyright (C) 2014, 2019 Niels Möller

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "ecc-internal.h"

/* Compute R <-- X^{2^k} mod M. Needs 2*ecc->size limbs at rp, and
   2*ecc->size additional limbs of scratch space. No overlap
   allowed. */
void
ecc_mod_pow_2k (const struct ecc_modulo *m,
		mp_limb_t *rp, const mp_limb_t *xp,
		unsigned k, mp_limb_t *tp)
{
  if (k & 1)
    {
      ecc_mod_sqr (m, rp, xp);
      k--;
    }
  else
    {
      ecc_mod_sqr (m, tp, xp);
      ecc_mod_sqr (m, rp, tp);
      k -= 2;
    }
  while (k > 0)
    {
      ecc_mod_sqr (m, tp, rp);
      ecc_mod_sqr (m, rp, tp);
      k -= 2;
    }
}

/* Computes R <-- X^{2^k + 1} * Y. Scratch requirements as ecc_mod_pow_2k. */
void
ecc_mod_pow_2k_mul (const struct ecc_modulo *m,
		    mp_limb_t *rp, const mp_limb_t *xp,
		    unsigned k, const mp_limb_t *yp,
		    mp_limb_t *tp)
{
  ecc_mod_pow_2k (m, tp, xp, k, rp);
  ecc_mod_mul (m, rp, tp, yp);
}
