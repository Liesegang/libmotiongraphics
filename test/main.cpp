#include "FFmpegReader.hpp"
#include "Exception.hpp"

#include <iostream>
#include <string>

#include <boost/format.hpp>

#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkCanvas.h"

void SaveGrayFrame(unsigned char *buf, int wrap, int xsize, int ysize, std::string filename)
{
    FILE *f;
    f = fopen(filename.c_str(), "w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    for (std::uint64_t i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

int main(int argc, char const *argv[])
{
    sk_sp<SkData> png = SkData::MakeFromFileName("C:/Test/out.png");
    sk_sp<SkImage> img = SkImage::MakeFromEncoded(png);

    sk_sp<SkSurface> rasterSurface = SkSurface::MakeRasterN32Premul(1920, 1080);
    SkCanvas* rasterCanvas = rasterSurface->getCanvas();
    rasterCanvas->drawImage(img, 0, 0);
    sk_sp<SkImage> img2(rasterSurface->makeImageSnapshot());
    if (!img2) { return 1; }
    sk_sp<SkData> png2(img2->encodeToData());
    if (!png2) { return 1; }
    SkFILEWStream out("C:/Test/out2.png");
    (void)out.write(png2->data(), png2->size());

    MotionGraphics::FFmpegReader* ffr = new MotionGraphics::FFmpegReader("C:/Test/test.mpg");
    try {
        ffr->Open();
        for (int i = 0; i < 10; i++) {
            MotionGraphics::Frame f = ffr->GetFrame();
            SaveGrayFrame(f.data, f.line, f.width, f.height, (boost::format("C:/Test/test-%02d.pgm") % i).str());
        }
        ffr->Close();
    }
    catch (MotionGraphics::ExceptionBase exception) {
        std::cout << exception.what() << std::endl;
    }
    return 0;
}
