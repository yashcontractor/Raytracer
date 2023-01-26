#include "glm/glm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

float NEAR = 0.0;
float LEFT = 0.0;
float RIGHT = 0.0;
float BOTTOM = 0.0;
float TOP = 0.0;
int RES_X = 0;
int RES_Y = 0;
float BACK_R = 0.0;
float BACK_G = 0.0;
float BACK_B = 0.0;
float AMBIENT_IR = 0.0;
float AMBIENT_IG = 0.0;
float AMBIENT_IB = 0.0;
char OUTPUT[25];
// Sphere calss
struct sphere
{
    char name[50];
    float posx;
    float posy;
    float posz;
    float sclx;
    float scly;
    float sclz;
    float r;
    float g;
    float b;
    float Ka;
    float Kd;
    float Ks;
    float Kr;
    int n;
} sphere[15];
// light class
struct light
{
    char name[25];
    float posx;
    float posy;
    float posz;
    float ir;
    float ig;
    float ib;
} light[10];
// check if camera ray intersects the sphere
bool intersect(const glm::vec3 &orig, const glm::vec3 &dir, int num, float &t)
{
    float radius2 = pow(sphere[num].sclx * 30, 2);
    glm::vec3 center = glm::vec3(sphere[num].posx * 30, sphere[num].posy * 30, sphere[num].posz);
    float a = glm::dot(dir, dir);
    float b = 2 * glm::dot((orig - center), dir);
    float c = glm::dot(orig - center, orig - center) - radius2;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0)
    {
        return false;
    }
    else
    {
        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);
        t = (t1 < t2) ? t1 : t2;
        if (t > 0.000001)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
// saves the PPM acording to the pixels
void save_imageP3(int Width, int Height, char *fname, unsigned char *pixels)
{
    FILE *fp;
    const int maxVal = 255;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname, "w");
    if (!fp)
    {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    int k = 0;
    for (int j = 0; j < Height; j++)
    {

        for (int i = 0; i < Width; i++)
        {
            fprintf(fp, " %d %d %d", pixels[k], pixels[k + 1], pixels[k + 2]);
            k = k + 3;
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int sphere_count = 0;
int light_count = 0;
int main(int argc, char *argv[])
{
    FILE *input_file;
    input_file = fopen(argv[1], "r");

    if (input_file == NULL)
    {
        printf("Cant open the file %s for reading.\n", argv[1]);
        exit(1);
    }

    char word[25];
    char *out;
    // reading Text File storing
    while (fscanf(input_file, "%s", word) != EOF)
    {

        // NEAR
        if (word[0] == 'N' && word[1] == 'E' && word[2] == 'A' && word[3] == 'R')
        {
            fscanf(input_file, "%s", word);
            NEAR = atof(word);
        }
        // LEFT
        if (word[0] == 'L' && word[1] == 'E' && word[2] == 'F' && word[3] == 'T')
        {
            fscanf(input_file, "%s", word);
            LEFT = atof(word);
        }
        // RIGHT
        if (word[0] == 'R' && word[1] == 'I' && word[2] == 'G' && word[3] == 'H' && word[4] == 'T')
        {
            fscanf(input_file, "%s", word);
            RIGHT = atof(word);
        }
        // BOTTOM
        if (word[0] == 'B' && word[1] == 'O' && word[2] == 'T' && word[3] == 'T' && word[4] == 'O' && word[5] == 'M')
        {
            fscanf(input_file, "%s", word);
            BOTTOM = atof(word);
        }
        // TOP
        if (word[0] == 'T' && word[1] == 'O' && word[2] == 'P')
        {
            fscanf(input_file, "%s", word);
            TOP = atof(word);
        }
        // RES
        if (word[0] == 'R' && word[1] == 'E' && word[2] == 'S')
        {
            fscanf(input_file, "%s", word);
            RES_X = atoi(word);
            fscanf(input_file, "%s", word);
            RES_Y = atoi(word);
        }

        // SPHERE
        if (word[0] == 'S' && word[1] == 'P' && word[2] == 'H' && word[3] == 'E' && word[4] == 'R' && word[5] == 'E')
        {
            fscanf(input_file, "%s", word);
            for (int i = 0; i < sizeof(word); i++)
            {
                sphere[sphere_count].name[i] = word[i];
            }
            fscanf(input_file, "%s", word);
            sphere[sphere_count].posx = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].posy = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].posz = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].sclx = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].scly = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].sclz = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].r = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].g = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].b = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].Ka = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].Kd = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].Ks = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].Kr = atof(word);
            fscanf(input_file, "%s", word);
            sphere[sphere_count].n = atoi(word);
            sphere_count++;
        }

        // LIGHT
        if (word[0] == 'L' && word[1] == 'I' && word[2] == 'G' && word[3] == 'H' && word[4] == 'T')
        {
            fscanf(input_file, "%s", word);
            for (int i = 0; i < sizeof(word); i++)
            {
                light[light_count].name[i] = word[i];
            }
            fscanf(input_file, "%s", word);
            light[light_count].posx = atof(word);
            fscanf(input_file, "%s", word);
            light[light_count].posy = atof(word);
            fscanf(input_file, "%s", word);
            light[light_count].posz = atof(word);
            fscanf(input_file, "%s", word);
            light[light_count].ir = atof(word);
            fscanf(input_file, "%s", word);
            light[light_count].ig = atof(word);
            fscanf(input_file, "%s", word);
            light[light_count].ib = atof(word);
            light_count++;
        }

        // BACK
        if (word[0] == 'B' && word[1] == 'A' && word[2] == 'C' && word[3] == 'K')
        {
            fscanf(input_file, "%s", word);
            BACK_R = atof(word);
            fscanf(input_file, "%s", word);
            BACK_G = atof(word);
            fscanf(input_file, "%s", word);
            BACK_B = atof(word);
        }

        // AMBIENT
        if (word[0] == 'A' && word[1] == 'M' && word[2] == 'B' && word[3] == 'I' && word[4] == 'E' && word[5] == 'N' && word[6] == 'T')
        {
            fscanf(input_file, "%s", word);
            AMBIENT_IR = atof(word);
            fscanf(input_file, "%s", word);
            AMBIENT_IG = atof(word);
            fscanf(input_file, "%s", word);
            AMBIENT_IB = atof(word);
        }

        // OUTPUT
        if (word[0] == 'O' && word[1] == 'U' && word[2] == 'T' && word[3] == 'P' && word[4] == 'U' && word[5] == 'T')
        {
            fscanf(input_file, "%s", word);
            for (int i = 0; i < sizeof(word); i++)
            {
                OUTPUT[i] = word[i];
            }
        }
    }
    fclose(input_file);

    int Width = RES_Y;
    int Height = RES_X;
    unsigned char *pixels;
    pixels = new unsigned char[3 * Width * Height];
    int k = 0;
    int i = 0;
    // goes through every pixel
    for (; i < Height; i++)
    {
        int j = 0;
        for (; j < Width; j++)
        {
            int a = 0;
            int count = 0;
            float r = 0.0;
            float b = 0.0;
            float g = 0.0;
            bool hit = false;
            // goes through every sphere to check if there is an intersect
            for (; count < sphere_count; count++)
            {
                float t;
                // sphere origin
                glm::vec3 shperecenter = glm::vec3(sphere[count].sclx, sphere[count].posy, sphere[count].posz);
                // check for intersect using origin and direction, if there is hit it set to true and the color is generated with antialiasing
                if (intersect(glm::vec3((j - Width / 2 + .5), -(i - Height / 2 + .5), 100), glm::vec3(0.0, 0.0, -1.0), count, t))
                {
                    hit = true;
                    // antialiasing 8 by 8
                    for (int row = 0; row < 8; row++)
                    {
                        for (int col = 0; col < 8; col++)
                        {
                            // check if the sphere is visable to the camera
                            bool seen = false;
                            if (sphere[count].posz < 0)
                            {
                                seen = true;
                            }
                            // the origin of the ray that will hit the sphere
                            glm::vec3 origin = glm::vec3((j - Width / 2 + (row + .5) / 8), -(i - Height / 2 + (col + .5) / 8), 100);
                            glm::vec3 dir = glm::vec3(0.0, 0.0, -1.0);
                            if (seen && intersect(origin, dir, count, t))
                            {
                                glm::vec3 point = origin + dir * float(t);
                                glm::vec3 L = glm::vec3(light[0].posx, light[0].posy, light[0].posz) - point;
                                glm::vec3 V = -point;
                                glm::vec3 N = (point - shperecenter) / sphere[count].sclx;
                                glm::vec3 R = 2 * (glm::dot(N, L)) * N - L;
                                double dt = glm::dot(glm::normalize(L), glm::normalize(N));
                                // the color of the pixel with ray-tracing
                                r += 255 * ((AMBIENT_IR * sphere[count].Ka + light[0].ir * sphere[count].Kd * (std::max(glm::dot(glm::normalize(N), glm::normalize(L)), 0.0f) + light[0].ir * sphere[count].Ks * pow(std::max(glm::dot(glm::normalize(R), glm::normalize(V)), 0.0f), sphere[count].n))) * sphere[count].r);
                                g += 255 * ((AMBIENT_IG * sphere[count].Ka + light[0].ig * sphere[count].Kd * (std::max(glm::dot(glm::normalize(N), glm::normalize(L)), 0.0f) + light[0].ig * sphere[count].Ks * pow(std::max(glm::dot(glm::normalize(R), glm::normalize(V)), 0.0f), sphere[count].n))) * sphere[count].g);
                                b += 255 * ((AMBIENT_IB * sphere[count].Ka + light[0].ib * sphere[count].Kd * (std::max(glm::dot(glm::normalize(N), glm::normalize(L)), 0.0f) + light[0].ib * sphere[count].Ks * pow(std::max(glm::dot(glm::normalize(R), glm::normalize(V)), 0.0f), sphere[count].n))) * sphere[count].b);
                            }
                            else
                            { // if the ray doesn't hit the sphere, then the color is the background color
                                r += 255 * BACK_R;
                                g += 255 * BACK_G;
                                b += 255 * BACK_B;
                            }
                        }
                    }
                }
            }
            // checKs if the ray hit any sphere
            if (!hit)
            {
                r = 255 * BACK_R;
                g = 255 * BACK_G;
                b = 255 * BACK_B;
            }
            else
            {
                // divides by 64 for antialiasing
                r /= 64;
                g /= 64;
                b /= 64;
            }
            // set the color of the pixel
            pixels[k] = r;
            pixels[k + 1] = g;
            pixels[k + 2] = b;
            k = k + 3;
        }
    }
    // saves a PPM file with the OUTPUT as the name
    save_imageP3(Width, Height, OUTPUT, pixels);
    return 0;
}
