#ifndef STRUCT
# define STRUCT

typedef struct	s_video
{
	double	id;
	double	creator_id;
	double	likes;
	double	dislikes;
}	t_video;

typedef struct	s_content_creator
{
	double	followers;
	double	creator_id;
	char	channel[256];
}	t_content_creator;

#endif
