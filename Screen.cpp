/* Abstract class detailing the interface with the graphics output.
 * Brian Scaramella 7/3/2016
 */

class Screen
{
  protected:
    int width;
    int height;
  public:
    Screen(int* width, int* height, char* bpp) = 0;
    ~Screen() = 0;

    virtual void draw_pixel(int x, int y, int c) = 0;
    virtual void draw_rect(int x, int y, int w, int h, int c) = 0;
//    virtual void put_image();

    virtual void clear_screen(int c) = 0;
    virtual void switch_page(void) = 0;
    virtual int test() = 0;
}
