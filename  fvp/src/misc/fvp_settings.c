/*
 * File: fvp_settings.c
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
 * q
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2011-3-22 zhoumin <dcdcmin@gmail.com> created
 *
 */
#include"fvp_settings.h"
#include"fvp_common.h"
#include"ini_builder.h"
#include"ini_parser.h"
#include"fvp_mmap.h"

struct _GroupNode;

#define MAX_COMMENT_BUF_SIZE	128

struct _FvpSettings
{
	struct _GroupNode *group_node;
	struct _GroupNode *current_group;
	char *settings_file_name;
	unsigned char have_comment_flag;
	char comment_buffer[MAX_COMMENT_BUF_SIZE];
};

typedef struct _PrivInfo
{
	FvpSettings *settings;
}PrivInfo;

#define COMMENT_CHAR "#" 		/*ini file with this char show that this line is the comment line*/

struct _KeyNode
{
	char *key_string;
	char *value_string;
	char *comment;
	struct _KeyNode *next_key_node;
};	

struct _GroupNode
{
	char *group_string;
	char *comment;
	struct _GroupNode *next_group;
	struct _KeyNode *key_node;
	struct _KeyNode *cur_node;
};

static struct _KeyNode *key_node_create(FvpSettings *settings, char *key, char *value)
{
	return_val_if_failed(settings && key , NULL);
	
	struct _KeyNode *key_node = (struct _KeyNode *)COMM_ZALLOC(sizeof(*key_node));

	key_node->key_string = COMM_STRDUP(key);
	key_node->value_string = COMM_STRDUP(value);

	/*if have a comment*/
	if(settings->have_comment_flag)
	{	
		key_node->comment = COMM_STRDUP(settings->comment_buffer);
		settings->have_comment_flag = 0;
	}

	return key_node;
}

static int groupNode_add_keyNode(struct _GroupNode *group_node, struct _KeyNode *key_node)
{
	if(group_node && key_node)
	{	
		/*if first key_node is null add to the first keynode*/
		if(group_node->key_node == NULL)
		{	
			group_node->key_node = key_node;
			group_node->cur_node = key_node;
		}
		else
		{
			group_node->cur_node->next_key_node = key_node;
			group_node->cur_node = key_node;
		}
		return 0;
	}
	return -1;
}

static int fvp_settings_add_key_value(FvpSettings *settings, char *key, char *value)
{
	return_val_if_failed(settings != NULL && key != NULL, -1);

	if(settings->group_node != NULL)
	{	
		/*create a key node*/
		struct _KeyNode *key_node = key_node_create(settings, key, value);	
		return_val_if_failed(key_node != NULL, -1);
		
		/*add the key node to the last group */
		struct _GroupNode *current_group = settings->current_group;
		
		if(current_group)
		{
			return groupNode_add_keyNode(current_group, key_node);
		}
	}
	
	return -1;
}

static int fvp_settings_add_group(FvpSettings *settings, char *group)
{
	return_val_if_failed(settings != NULL && group != NULL, -1);

	
	/*create a group node*/
	struct _GroupNode *group_node  = (struct _GroupNode*)COMM_ZALLOC(sizeof(*group_node));
	group_node->group_string = COMM_STRDUP(group);
	group_node->key_node = NULL;
	if(settings->have_comment_flag)
	{	
		group_node->comment = COMM_STRDUP(settings->comment_buffer);
		settings->have_comment_flag = 0;
	}
	
	/*add the group to group list*/
	if(settings->group_node == NULL)
	{
		settings->group_node = group_node;
		settings->current_group = group_node;
	}
	else
	{
		settings->current_group->next_group = group_node;
		settings->current_group = group_node;
	}
	
	return 0;
}


static int fvp_settings_store_comment(FvpSettings *settings, char *comment)
{
	return_val_if_failed(settings && comment, -1);

	
	settings->have_comment_flag = 1;

	strncpy(settings->comment_buffer, comment, MAX_COMMENT_BUF_SIZE);
	
	return 0;
}
static void  fvp_settings_builder_on_group(IniBuilder *thiz, char *group)
{
	return_if_failed(thiz != NULL && group != NULL);
	
	DECL_PRIV(thiz, priv);
	
	fvp_settings_add_group(priv->settings, group);
	return;
}


static void fvp_settings_builder_on_key_value(IniBuilder *thiz, char *key, char *value)
{
	return_if_failed(thiz != NULL && key != NULL);

//	printf("%s=%s\n", key, value);
	
	DECL_PRIV(thiz, priv);
	
	fvp_settings_add_key_value(priv->settings, key, value);
	return;
}

static void fvp_settings_builder_on_comment(IniBuilder *thiz, char *comment)
{
	return_if_failed(thiz != NULL && comment != NULL);

	DECL_PRIV(thiz, priv);
	
	fvp_settings_store_comment(priv->settings, comment);
	
	return;
}

static void fvp_settings_builder_destroy(IniBuilder *thiz)
{
	if(thiz)
	{
		COMM_ZFREE(thiz, sizeof(thiz) + sizeof(PrivInfo));
	}
}

static IniBuilder *fvp_settings_builder_create(void)
{
	IniBuilder *thiz = COMM_ALLOC(sizeof(IniBuilder) + sizeof(PrivInfo)); 
	if(thiz != NULL)
	{
		thiz->on_group = fvp_settings_builder_on_group;
		thiz->on_key_value = fvp_settings_builder_on_key_value;
		thiz->on_comment = fvp_settings_builder_on_comment;
		thiz->destroy = fvp_settings_builder_destroy;			
	}

	return thiz;
}


static  int fvp_settings_parser_file(FvpSettings *thiz)
{
	return_val_if_failed(thiz != NULL && thiz->settings_file_name != NULL, -1);
	
	IniParser *parser = NULL;
	IniBuilder *builder = NULL;
	FvpMmap *ini_mmap = NULL;
	
	/*create the ini parser*/
	parser = ini_parser_create();
	ini_mmap = fvp_mmap_create(thiz->settings_file_name, 0, -1);
	if(ini_mmap == NULL)
	{
		ini_parser_destory(parser);
		return -1;
	}
	
	/*create the builder*/
	builder = fvp_settings_builder_create();
	if(builder == NULL)
	{		
		fvp_mmap_destroy(ini_mmap);
		ini_parser_destory(parser);
		return -1;
	}	
	PrivInfo* priv = (PrivInfo *)builder->priv;
	priv->settings = thiz;
	
	ini_parser_set_builder(parser, builder);
	/*parser the ini file*/
	char *data = fvp_mmap_data(ini_mmap);
	ini_parser_parse(parser, data, COMMENT_CHAR);

	/*destroy the parser and builder*/
	fvp_mmap_destroy(ini_mmap);
	ini_parser_destory(parser);
	ini_builder_destroy(builder);
	
	return 0;
}		

FvpSettings *fvp_settings_create(char *settings_file)
{	
	return_val_if_failed(settings_file != NULL, NULL);
	
	FvpSettings *thiz = NULL;

	thiz = (FvpSettings *)COMM_ZALLOC(sizeof(FvpSettings));
	return_val_if_failed(thiz != NULL, NULL);
	
	thiz->settings_file_name = COMM_STRDUP(settings_file);
	
	thiz->current_group = NULL;
	thiz->group_node = NULL;
	/*parser the file*/
	fvp_settings_parser_file(thiz);

	return  thiz;
}

char *fvp_settings_get_value(FvpSettings *thiz, char *group, char *key)
{
	return_val_if_failed(thiz && group && key, NULL);

	struct _GroupNode *group_node = thiz->group_node;

	for(;group_node != NULL; group_node = group_node->next_group)
	{	
		if(strcmp(group_node->group_string, group) == 0)
		{	
			/*find the group node */
			struct _KeyNode *key_node = group_node->key_node;
			for(;key_node ;key_node = key_node->next_key_node)
			{
				/*find the key node*/
				if(strcmp(key_node->key_string, key) == 0)
				{
					printf("comment(%s)\n", key_node->comment);
					return key_node->value_string;
				}
			}
		}
	}
	
	return NULL;
}

void fvp_settings_destroy(FvpSettings *thiz)
{
	if(thiz)
	{
		COMM_FREE(thiz->settings_file_name);

		struct _GroupNode *group_node = thiz->group_node;
		struct _GroupNode *temp_group_node;
		
		for( ;group_node != NULL; )
		{
			struct _KeyNode *key_node = group_node->key_node;
			struct _KeyNode *temp_key_node = NULL;

			for( ;key_node ; )
			{
				temp_key_node = key_node;
				key_node = key_node->next_key_node;
				
				COMM_FREE(temp_key_node->key_string);
				COMM_FREE(temp_key_node->value_string);
				COMM_FREE(temp_key_node->comment);
				COMM_FREE(temp_key_node);
			}
			
			temp_group_node = group_node;
			group_node = group_node->next_group;

			COMM_FREE(temp_group_node->group_string);
			COMM_FREE(temp_group_node->comment);
			COMM_FREE(temp_group_node);
		}		
		
		COMM_ZFREE(thiz, sizeof(FvpSettings));
	}	
	
	return;
}

