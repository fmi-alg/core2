#include "NewWindow.h"
#include "ui_NewWindow.h"
#include <QPainter>
#include <iostream>
#include <fstream>
#include <ControlWindow.h>
#include <QCursor>
#include <QMouseEvent>
#include <cmath>



/*  This is the window for creating input files. The user clicks a button to add a polygon,
*   selectes the number of vertices, and then decides whether it will be an "interior" or
*   "exterior" polygon. As of right now, only one "exterior" polygon is possible (in theory this makes sense,
*   how could you have multiple exterior obstacles?).
*   The user draws the polygons by clicking a screen to create points. The screen is implemented
*   using QPainter.
*   Written by John Ryan - jpr349@nyu.edu
*/






std::string file;

void showAgain(std::string str);     //function to reshow Control Window
bool isEmpty(int arr[]);                //determines if array is all 0s
void deleteAllPolygons();               //for clearing the polygon data structure
void addPolygon(int index);               //Adds polygon from data structure of points into data structure of polygons

std::string output;                 //these strings are for the output file
std::string points;                 //see the README for input formatting
std::string order;
std::string path;
bool backgroundChange;              //has an exterior polygon been made? IE should the background be yellow?
bool interior;                      //is the current polygon an interior polygon?
int allPolygons[20][10][2];            //data structure containing polygon points (max 20 polygons, max 10 sides each)
int numberOfVertices;                  //number of vertices of current polygon
int totalPoints;                    //total number of points that have been placed
int currentVertex;                  //we're placing the nth vertex of the current polygon
int currentPolygon;                     //placing the nth polygon
bool clickable;                         //is the user clicking the screen now?
QPolygon arr[20];                       //array of polygons (for filling in with color)
QPainterPath* tmp;                  //path to contain polygons for filling in with color
QPainterPath* gray;                 //path to be filled gray if exterior polygon is made


QPen pen;
short polyAdded[20];                    //each entry is 1 if the nth poly has been added, 0 otherwise

NewWindow::NewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewWindow)
{
    ui->setupUi(this);


    ui->yes->hide();                                //set up the window with certain buttons hidden, others disabled, etc.
    ui->no->hide();
    ui->label_3->setEnabled(false);
    ui->label_4->hide();
    ui->label_5->hide();
    ui->spinBox->setEnabled(false);
    ui->exterior->hide();
    ui->interior->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->note->hide();
    ui->ok->hide();
    ui->exterior->setEnabled(true);
    currentVertex=0;
    currentPolygon=0;
    totalPoints=0;
    clickable=false;
    backgroundChange=false;
    tmp=new QPainterPath();
    gray=new QPainterPath();

    output="";
    points="";
    order="";

    for(int a=0;a<20;a++) polyAdded[a]=0;      //no polygons added yet

}

NewWindow::~NewWindow()                     //this could probably use work
{
    deleteAllPolygons();
    delete ui;
}

//================================//
//     Display Text to Window     //
//================================//
/* Scrolls the text screen to the bottom and prints text */
NewWindow& NewWindow::operator<< (const std::string& str) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(str.c_str());
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

NewWindow& NewWindow::operator<< (const char* text) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(text);
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

NewWindow& NewWindow::operator<< (int i) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(i));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

NewWindow& NewWindow::operator<< (long l) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(l));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

NewWindow& NewWindow::operator<< (float f) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(f));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

NewWindow& NewWindow::operator<< (double d) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(d));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}






void NewWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);

    painter.drawRect(10,10,512,512);                            //screen for adding vertices
    if(backgroundChange){                                       //background gray if no interior polygon yet, yellow otherwise
        painter.fillRect(10,10,512,512,Qt::yellow);
    }else{
        painter.fillRect(10,10,512,512,Qt::gray);
    }
    pen.setBrush(Qt::yellow);

    pen.setWidth(1);
    painter.setPen(pen);
    int x,y,b;


    if(clickable) painter.fillPath(*gray,Qt::gray);                 //if exterior polygon has happened, this colors the interior
                                                                     //of that polygon gray, and the interior can have vertices
                                                                    //put on top of it
    for(int a=0;a<=currentPolygon;a++){
        for(b=0; b<10  &&  !isEmpty(allPolygons[a][b]);    b++){
            x=allPolygons[a][b][0];
            y=allPolygons[a][b][1];

            if(a==currentPolygon) painter.drawEllipse(x+9,y+9,2,2);                 //for current polygon, make the vertices circles
        }                                           //the points have 9 added because the screen for clicking is 10
                                                    //pixels down and right in the main window. 9 because the circles have
                                                    //radius 1 and we want the click to be the center of the circle


        if(  (!clickable  ||   a!=currentPolygon )){            //when done clicking or if polygon already added, put it
                                                                //in the datastructure of polygons for filling
            addPolygon(a);

        }
    }
    if(!clickable) painter.fillPath(*gray,Qt::gray);            //if exterior has happened, this colors the interior gray
                                                                //this happens when !clickable AFTER the above for loop
                                                                //because, when clickable, the vertices should be visible
                                                                //OVER the gray interior
    painter.fillPath(*tmp,Qt::yellow);                  //fill the interior polygons with yellow


}

void addPolygon(int index){
    if(isEmpty(allPolygons[index][0])) return;
    if(polyAdded[index]) return;

    polyAdded[index]=1;
    QPolygon *current = new QPolygon(numberOfVertices);

    for(int x=0;x<numberOfVertices;x++){
        if(isEmpty(allPolygons[index][x])) break;
        current->setPoint(x,allPolygons[index][x][0]+10,allPolygons[index][x][1]+10);
    }

    if(interior){
        tmp->addPolygon(*current);
    }else{
        gray->addPolygon(*current);
    }



}



bool isEmpty(int arr[]){
    return (arr[0]==0&&arr[1]==0);
}

void NewWindow::mousePressEvent(QMouseEvent *location){
    if(!clickable) return;
    int x = location->x()-10;
    int y = location->y()-10;
    if(x<0||y<0||x>512||y>512) return;
    totalPoints++;
    points+=std::to_string(x)+" "+std::to_string(y)+"\n";
    order+=std::to_string(totalPoints)+" ";
        allPolygons[currentPolygon][currentVertex][0]=x;
        allPolygons[currentPolygon][currentVertex][1]=y;


    this->update();
    if(currentVertex==numberOfVertices-1){
        if(!interior){
            backgroundChange=true;
        }
        order+=std::to_string(totalPoints-numberOfVertices+1)+"\n";
        currentVertex=0;
        currentPolygon++;
        clickable=false;
        ui->label_3->setEnabled(false);
        ui->label_4->hide();
        ui->label_5->hide();
        ui->spinBox->setEnabled(false);
        ui->exterior->hide();
        ui->interior->hide();
        if(!backgroundChange) ui->exterior->setEnabled(true);
        ui->interior->setEnabled(true);
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->ok->hide();
        ui->ok->setEnabled(true);
        ui->interior->setChecked(false);
        ui->exterior->setChecked(false);
        return;
    }
    currentVertex++;
}
















void NewWindow::on_pushButton_clicked()
{
    this->close();
    backgroundChange=false;
    deleteAllPolygons();
    showAgain("");
}




void NewWindow::on_pushButton_3_clicked()
{

    output+=std::to_string(totalPoints)+"\n";
    output+=points;
    output+=std::to_string(currentPolygon)+"\n";
    output+=order;

    file = ui->lineEdit->text().toStdString()+".txt";
    if(file.size()==4){
        *this<<"Please enter a name for the file.\n";
        *this<<QDir::currentPath().toStdString();
        return;
    }
    path = "inputs/"+file;

    if(std::ifstream(path)){
        *this<<"A file by that name already exists. Overwrite?\n";
        ui->yes->show();
        ui->no->show();
        return;
    }

    showAgain(file);
    this->close();
    backgroundChange=false;
    deleteAllPolygons();
    std::ofstream newFile;
    newFile.open(path);
    newFile<<output;
    newFile.close();

}

void NewWindow::on_yes_clicked()
{
    this->close();
    backgroundChange=false;
    deleteAllPolygons();
    showAgain(file);
    std::ofstream newFile;
    newFile.open(path);
    newFile<<output;
    newFile.close();

}

void NewWindow::on_no_clicked()
{
    ui->yes->hide();
    ui->no->hide();
    *this<<"Please rename the file.\n";
}

void NewWindow::on_pushButton_4_clicked()
{
    ui->spinBox->setEnabled(true);
    ui->ok->show();
    ui->label_3->setEnabled(true);
}

void NewWindow::on_ok_clicked()
{
    numberOfVertices=ui->spinBox->value();
    ui->ok->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->label_3->setEnabled(false);
    ui->label_4->show();
    ui->label_5->show();
    ui->exterior->show();
    ui->interior->show();
    ui->note->show();

}

void NewWindow::on_interior_clicked()
{
    ui->note->hide();
    interior=true;
    ui->interior->setChecked(true);
    ui->exterior->setEnabled(false);
    ui->interior->setEnabled(false);
    ui->label_6->show();
    ui->label_8->show();
    clickable=true;
}

void NewWindow::on_exterior_clicked()
{
    ui->note->hide();
    interior=false;
    ui->exterior->setChecked(true);
    ui->exterior->setEnabled(false);
    ui->interior->setEnabled(false);
    ui->label_7->show();
    ui->label_8->show();
    clickable=true;
}

void deleteAllPolygons(){
    int x;
    for(x=0;x<20;x++){
        for(int y=0;y<10;y++){
            allPolygons[x][y][0]=0;
            allPolygons[x][y][1]=0;
        }
    }


}



void NewWindow::on_pushButton_2_clicked()
{
    backgroundChange=false;
    deleteAllPolygons();
    this->close();
    NewWindow *inputWin = new NewWindow();
    inputWin->show();
    this->~NewWindow();
}
