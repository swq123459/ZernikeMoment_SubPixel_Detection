#ifndef WINDOWPARAM_H
#define WINDOWPARAM_H

#include <gtk/gtk.h>


/*
ZerResult CalDistanceSquare(Mat& matRoi, double rto = MMPERPIXEL,
        int drawColor = WHITE, bool ifmedianBlur=YESMEDIANBLUR,
        int cannyL = 150, int cannyH = 200, int cannyCore = 3,
        int nbsize = N, Point2i VHtime = Point2i(4, 4),
        int ZerBgrL = 10, int ZerBgrH = 50

    );
ZerResultCir CalDistanceCircle(Mat& matRoi, double rto = MMPERPIXEL,
        int drawColor = WHITE, bool ifmedianBlur = YESMEDIANBLUR,
        int cannyL = 150, int cannyH = 200, int cannyCore = 3,
        int nbsize = N, Point2i VHtime = Point2i(4, 4),
        int ZerBgrL = 10, int ZerBgrH = 50

    );
*/

typedef struct tag_paramSet{
    int cannyL;
    int cannyH;
    int cannyCore;
    int ZerBgrL;
    int ZerBgrH;
    int DrawColor;
    int neighborSize;
    bool useMedium;
    int VScale;
    int HScale;
}ParamSet;

void Initial_WindowParam(ParamSet *ext_params);
void Show_WindowParam();
void Hide_WindowParam();

void Fill_DefaultParams();
void Use_DefaultParams(ParamSet* params);

void SetParamsFromView(ParamSet* params);
void FillParamsToView(ParamSet *params);


void on_btn_apply_clicked(GtkWidget* button,gpointer userdata);
void on_btn_cancel_clicked(GtkWidget* button,gpointer userdata);
void on_btn_default_clicked(GtkWidget* button,gpointer userdata);

void on_checkBox_meida_toggle(GtkWidget *check_button, gpointer data);

#endif // WINDOWPARAM_H
