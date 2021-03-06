#include <orientation.h>
#include <math.h>
#include <point3d.h>

ORIENTATION::ORIENTATION(double x, double y, double z, double yaw, double pitch, double roll, double height, double weight,double focus)
{
    mainPoint = creatPoint(x,y,z);
    yawBla = toRadian( yaw);
    pitchBla = toRadian( pitch);
    rollBla = toRadian(roll);
    cameraHeight = mmToMeters(height);
    cameraWidht  = mmToMeters(weight);
    cameraFocus  = mmToMeters(focus);
    rotartion();
    findAnglPoint();
}

ORIENTATION::~ORIENTATION(){
    //what do ???
}

double ORIENTATION::toRadian(double angle){
    return angle*M_PI/180;
}

double ORIENTATION::smToMeters(double sm){
    return sm/100;
}

double ORIENTATION::mmToMeters(double mm){
    return smToMeters(mm/10);
}

void ORIENTATION ::findAnglPoint(){
    // головна точка
    O1.x = mainPoint.x;
    O1.y = mainPoint.y;
    O1.z = 0;
    // обрахунок
    double OB = (mainPoint.z * cameraWidht)  / (2 * cameraFocus);
    double OA = (mainPoint.z * cameraHeight) / (2 * cameraFocus);
    //кутові точки
    //координата по х
    P1.x = + OB;
    P2.x = - OB;
    P3.x = - OB;
    P4.x = + OB;
    //кооридната по у
    P1.y = + OA;
    P2.y = + OA;
    P3.y = - OA;
    P4.y = - OA;

    //after turn
    P1 = multiplicatePoint(P1);
    P2 = multiplicatePoint(P2);
    P3 = multiplicatePoint(P3);
    P4 = multiplicatePoint(P4);

    //add coordinate UAV
    P1 = add(P1, O1);
    P2 = add(P2, O1);
    P3 = add(P3, O1);
    P4 = add(P4, O1);

}

ORIENTATION::point  ORIENTATION::multiplicatePoint(point a){
    point b;
    b.x =mulMatrix[0][0]*a.x+mulMatrix[1][0]*a.y+mulMatrix[2][0]*a.z;
    b.y =mulMatrix[0][1]*a.x+mulMatrix[1][1]*a.y+mulMatrix[2][1]*a.z;
    b.z =mulMatrix[0][2]*a.x+mulMatrix[1][2]*a.y+mulMatrix[2][2]*a.z;
    return b;
}


void ORIENTATION::rotartion(){
    // = 0
    for(int i = 0; i<3; i++)
        for(int j = 0; j<3; j++){
            matrixYaw[i][j]   = 0;
            matrixRoll[i][j]  = 0;
            matrixPitch[i][j] = 0;
            mulMatrix[i][j] = 0;
            mulMatrix1[i][j] = 0;
        }
    //matrix Yaw in radian
    matrixYaw[2][2] = 1;
    matrixYaw[0][0] = cos(yawBla);
    matrixYaw[0][1] = -sin (yawBla);
    matrixYaw[1][0] = sin(yawBla);
    matrixYaw[1][1] = cos(yawBla);
    //matrix pitch in radian
    matrixPitch[1][1] = 1;
    matrixPitch[0][0] = cos(pitchBla);
    matrixPitch[0][2] = -sin(pitchBla);
    matrixPitch[2][0] = sin(pitchBla);
    matrixPitch[2][2] = cos(pitchBla);
    //matrix Roll in radian
    matrixRoll[0][0] = 1;
    matrixRoll[1][1] = cos(rollBla);
    matrixRoll[1][2] = -sin(rollBla);
    matrixRoll[2][1] = sin(rollBla);
    matrixRoll[2][2] = cos(rollBla);

    multiplicateMatrix1(matrixYaw,matrixPitch);
    multiplicateMatrix(mulMatrix1,matrixRoll);

}

void ORIENTATION::multiplicateMatrix1(double a[3][3], double b[3][3]){
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++){
            mulMatrix1[i][j] = 0;
            for (int k = 0; k<3; k++)
                mulMatrix1[i][j] = mulMatrix1[i][j] + a[i][k]*b[k][j];
        }
}

void ORIENTATION::multiplicateMatrix(double a[3][3], double b[3][3]){
    for (int i= 0; i<3; i++)
        for (int j= 0; j<3; j++){
            mulMatrix[i][j] = 0;
            for (int k = 0; k<3; k++)
                mulMatrix[i][j] = mulMatrix[i][j] + a[i][k]*b[k][j];
        }
}

void ORIENTATION::printDani(CvArr *img, int width){
    CvFont font;
    int xOut = width - 300;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, 9);
    cvPutText(img, toText(P1).toLatin1().data(), cvPoint(xOut,10), &font,CV_RGB(0,0,250));
    cvPutText(img, toText(P2).toLatin1().data(), cvPoint(xOut,25), &font,CV_RGB(0,0,250));
    cvPutText(img, toText(P3).toLatin1().data(), cvPoint(xOut,40), &font,CV_RGB(0,0,250));
    cvPutText(img, toText(P4).toLatin1().data(), cvPoint(xOut,55), &font,CV_RGB(0,0,250));
}

QString ORIENTATION::toText(point A){
    QString text =  QString::number(A.x,'f');
    QString X ="X = ";
    X = X + text;
    text = QString::number(A.y, 'f');
    QString Y ="  Y = ";
    Y = Y + text;
    return X = X + Y;
}

ORIENTATION::point ORIENTATION::add(point A, point B){
    A.x += B.x;
    A.y += B.y;
    A.z += B.z;
    return A;
}


ORIENTATION::point ORIENTATION::creatPoint(double x, double y, double z){
    point retutnPoint;
    retutnPoint.x = x;
    retutnPoint.y = y;
    retutnPoint.z = z;
    return retutnPoint;
}
