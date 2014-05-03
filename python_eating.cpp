#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include <curses.h>

class python_eating
{
      python_eating(const python_eating&a){}
	public:
		python_eating(int width,int height)
		{
			this->direct = this->head_x=this->head_y=this->tail_x=this->tail_y=0;
			this->width = width&0X7FFFFFFF;
			this->height = height&0X7FFFFFFF;
			this->matrix = (unsigned char*) malloc(sizeof(unsigned char)* this->width* this->height);
			if(this->matrix==NULL)
			{
				printf("malloc failed\n");
				exit(0);
			}
			memset(this->matrix,0,sizeof(char)* this->width* this->height);
			init();
		}
		~python_eating()
		{
			free(this->matrix);
		}
		void init()
		{
            //Start point
            this->matrix[0] = 1;
            this->head_x = this->head_y = this->tail_x = this->tail_y = 0;
            this->draw_char(0,0,'0');
		}
		int index(int x,int y)
		{
			if(this->matrix==NULL || x<0 || x>=this->width || y<0 || y>=this->height) return -1;
			return y*this->width+x;
		}
		int step()
		{
			int dx[] = {1,0,0,-1};
			int dy[] = {0,-1,1,0};
			int tx = this->head_x + dx[this->direct];
			int ty = this->head_y + dy[this->direct];
			int t_index = this->index(tx,ty);
			if(t_index <0) return 0;
			int c = this->matrix[t_index];
			if(c>0 && c<10) return 0;

            {
			//head add
                this->matrix[this->index(this->head_x,this->head_y)]=this->direct;
                this->head_x = tx;
                this->head_y = ty;
                this->matrix[t_index] = 1;
                draw_char(this->head_x,this->head_y,'0');
            }

            if(c==0)
            {
			//tail sub
                t_index = this->index(this->tail_x,this->tail_y);
                int tail_d = this->matrix[t_index]&3;
                this->matrix[t_index] = 0;
                draw_char(this->tail_x,this->tail_y,' ');
                this->tail_x += dx[tail_d];
                this->tail_y += dy[tail_d];
                add_food();
			}
			return 1;
		}
		void set_direct(int d)
		{
			d = d&3;
			(this->direct^d)!=3 ? this->direct = d : 0;
		}

		void draw_char (int x,int y,int c)
		{
            //fprintf(stderr,"x=%d,y=%d,c='%c'\n",x,y,c);
            move(x,y);
            addch(c);
		}

		void add_food()
		{
            ;
        }
	private:
		unsigned char *matrix;
		int width,height,direct;
		int head_x,head_y,tail_x,tail_y;
};

void *step_by_step(void* p)
{
    python_eating* pe = (python_eating* )p;
}

int main()
{
initscr();
    python_eating a(50,50);
    while(a.step())
    {
        usleep(1000*100);
    refresh();
        ;
    }
	return 0;
}
