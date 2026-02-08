/*
output a ppm (portable pixmap) image
*/
#include <iostream>
#include <fstream>

int main(){
    std::ofstream outFile("new_output.ppm", std::ios::out);

    int nx = 200;
    int ny = 100;
    outFile << "P3\n" << nx << " " <<ny << "\n255\n";
    for (int g=0 ; g < ny; g++){
        for (int r=0; r< nx ; r++){
            float r_val = float(r)/float(nx);
            float g_val = float(g)/float(ny);
            float b_val = 0.2;
            int ir = static_cast<int>(255 * r_val);
            int ig = static_cast<int>(255 * g_val);
            int ib = static_cast<int>(255 * b_val);
            outFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    outFile.close();
}