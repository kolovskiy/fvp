include $(srcdir)/rules.mk

SUBDIRS=. test

lib_LTLIBRARIES=libmedia.la

COMMON_HEADER=$(srcdir)/common/fvp_common.h \
			  $(srcdir)/common/fvp_config.h \
			  $(srcdir)/common/parameter_define.h 

MISC_SRC=$(srcdir)/misc/array_list.c \
		 $(srcdir)/misc/array_list.h \
		 $(srcdir)/misc/block.c \
		 $(srcdir)/misc/block.h \
		 $(srcdir)/misc/fvp_function.c\
		 $(srcdir)/misc/fvp_function.h \
		 $(srcdir)/misc/fvp_lock.c \
		 $(srcdir)/misc/fvp_lock.h \
		 $(srcdir)/misc/fvp_mmap.c \
		 $(srcdir)/misc/fvp_mmap.h \
		 $(srcdir)/misc/fvp_msg.c \
		 $(srcdir)/misc/fvp_msg.h \
		 $(srcdir)/misc/fvp_screen.c \
		 $(srcdir)/misc/fvp_screen.h \
		 $(srcdir)/misc/fvp_settings.c \
		 $(srcdir)/misc/fvp_settings.h\
		 $(srcdir)/misc/ini_builder.c\
		 $(srcdir)/misc/ini_builder.h\
		 $(srcdir)/misc/ini_parser.c\
		 $(srcdir)/misc/ini_parser.h


libmedia_la_SOURCES = mpp_system.c \
					mpp_system.h \
					media_player.c \
					media_player.h \
					media_player_event_manager.h \
					media_player_event_manager.c \
					./access/access.h \
					./access/access_avi.c \
					./access/access_avi.h \
					./access/access_factory.c \
					./access/access_factory.h \
					./access/access_file.c \
					./access/access_file.h \
					./decoder/video_decoder.c \
					./decoder/video_decoder.h \
					./decoder/audio_decoder.c \
					./decoder/audio_decoder.h \
					./video_windows/video_windows.c \
					./video_windows/video_windows.h \
					./chips/ad_codec.c \
					./chips/ad_codec.h \
					./audio_device/audio_output_device.c \
					./audio_device/audio_output_device.h \
					./fvp.c \
					./fvp.h \
					./fvp_global.c \
					./fvp_global.h 


libmedia_la_SOURCES +=$(COMMON_HEADER) $(MISC_SRC)	


libmedia_la_LDFLAGS = -lpthread -lm


libmedia_la_LIBADD=$(MYLDFLAGS)

mediadir=$(includedir)
media_HEADERS=$(srcdir)/common/fvp_common.h \
			  $(srcdir)/common/fvp_config.h \
			  $(srcdir)/common/fvp_typedef.h\
			  $(srcdir)/common/parameter_define.h \
			 $(srcdir)/misc/array_list.h \
			 $(srcdir)/misc/block.h \
			 $(srcdir)/misc/fvp_function.h \
		 $(srcdir)/misc/fvp_lock.h \
		 $(srcdir)/misc/fvp_mmap.h \
		 $(srcdir)/misc/fvp_msg.h \
		 $(srcdir)/misc/fvp_screen.h \
		 $(srcdir)/misc/fvp_settings.h\
		 $(srcdir)/misc/ini_builder.h\
		 $(srcdir)/misc/ini_parser.h	\
		mpp_system.h \
		media_player.h \
		./access/access.h \
		./access/access_avi.h \
		./access/access_factory.h \
		./access/access_file.h \
		./decoder/video_decoder.h \
		./decoder/audio_decoder.h \
		./video_windows/video_windows.h \
		./media_player_event_manager.h \
		./chips/ad_codec.h \
		./audio_device/audio_output_device.h \
		./fvp.h \
		./fvp_global.h  \
		$(top_srcdir)/driver/tw2865/tw2865.h \
		$(top_srcdir)/driver/tw2865/tw2865_def.h 



#INCLUDES= -I$(top_srcdir)/lib/mpp/include \
#		  -I$(top_srcdir)/lib/ffmpeg/include \
#		  -I$(top_srcdir)/src/misc \
#		  -I$(top_srcdir)/src/common \
#		  -I$(top_srcdir)/src/access \
#		  -I$(top_srcdir)/src/decoder \
#		  -I$(top_srcdir)/src/video_windows\
#		  -I$(top_srcdir)/src/audio_device \
#		  -I$(top_srcdir)/src/chips \
#		  -I$(top_srcdir)/driver/tw2865
