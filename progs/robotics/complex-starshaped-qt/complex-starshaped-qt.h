#include <QDir>
#include <QSurfaceFormat>
#include "QApplication"


void parseExampleList(){

    //    8/24/2016: Tom
    //               new way to parse files in a folder
    string cfgDir = workingDir + "/" + inputDir;
    QDir tmpDir(cfgDir.c_str());
    QStringList fileList = tmpDir.entryList();
    while(!fileList.empty()){
        QString fileName_l = fileList.front();
        strcpy(sptr, fileName_l.toStdString().c_str());
        int len = strlen(sptr);
        if(len > 4 && sptr[len-1] == 'g' && sptr[len-2] == 'f' && sptr[len-3] == 'c' && sptr[len-4] == '.'){
            strcpy(cfgNameList[numEg], sptr);
            ++numEg;
        }
        fileList.pop_front();
    }

//    sprintf(cfgPath, "ls -R > tmpList");
//    system(cfgPath);
//    sprintf(cfgPath, "tmpList");
//    FILE *fptr = fopen(cfgPath, "r");
//    if(fptr == NULL) return ;
//    while(fgets(tmp, 200, fptr) != NULL){
//        char *sptr = strtok(tmp, " \n");
//        while(sptr != NULL){
//            int len = strlen(sptr);
//            if(len > 3 && sptr[len-1] == 'g' && sptr[len-2] == 'e' && sptr[len-3] == '.'){
//                strcpy(cfgNameList[numEg], sptr);
//                ++numEg;
//            }
//            sptr = strtok(NULL, " \n");
//        }
//    }
//    sprintf(cfgPath, "rm -rf tmpList");
//    system(cfgPath);
}

