#ifndef STRUCT
# define STRUCT

typedef struct	s_video
{
	double	id;
	double	creator_id;
	char	video_name[256];
}	t_video;

typedef struct	s_content_creator
{
	double	creator_id;
	char	channel[256];
}	t_content_creator;

#endif
