/*
 * File: fb_test.c
 * Author:  zhoumin  <dcdcmin@gmail.com>
 * Brief:   
 *
 * Copyright (c) 2010 - 2013  zhoumin <dcdcmin@gmail.com>>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2011-3-24 zhoumin <dcdcmin@gmail.com> created
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"framebuffer.h"
#include"fvp_common.h"


int main(int argc, char *argv[])
{
	FrameBuffer *thiz  = NULL;

	thiz = framebuffer_create("/dev/fb0");
	assert(thiz != NULL);
	assert(framebuffer_init(thiz, 1024, 768) == RET_OK);

	getchar();
	framebuffer_change_alpha_value(thiz, 0x00);

	getchar();
	framebuffer_destroy(thiz);
	
	return 0;
}
