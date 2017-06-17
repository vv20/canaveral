#include <QString>
#include <QVector>
#include "sample.h"

Sample::Sample (QString file) {
  if (file == NULL) {
    filename = "---";
    samplename = "---";
  }
  else {
    filename = file;

    QVector<QStringRef> split = file.splitRef("/");
    samplename = split.last().toString();
  }
}

QString Sample::getFilename () {
  return filename;
}

QString Sample::getSamplename () {
  return samplename;
}

