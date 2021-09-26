/*
 * File: graphics.h
 * ----------------
 * This interface provides access to a simple graphics library that makes
 * it possible to draw lines, rectangles, ovals, arcs, polygons, images,
 * and strings on a graphical window.
 */

#ifndef _graphics_h
#define _graphics_h

#include <string>
#include "gtypes.h"
#include "vector.h"

/*
 * Function: initGraphics
 * Usage: initGraphics();
 *        initGraphics(width, height);
 * -----------------------------------
 * Creates the graphics window on the screen.  The first form creates a
 * window with a default size of 500x300; the second allows the client to
 * specify the size of the window.  The call to initGraphics must precede
 * any console output or calls to other functions in this interface.
 */

void initGraphics();
void initGraphics(int width, int height);

/*
 * Function: drawArc
 * Usage: drawArc(bounds, start, sweep);
 *        drawArc(x, y, width, height, start, sweep);
 * --------------------------------------------------
 * Draws an elliptical arc inscribed in a rectangle.   The parameters x, y,
 * width, and height (or, equivalently, the GRectangle bounds) specify the
 * coordinates and dimensions of the bounding rectangle.  The start
 * parameter indicates the angle at which the arc begins and is measured in
 * degrees counterclockwise from the +x axis.  Thus, a start angle of 0
 * indicates an arc that begins along the line running eastward from the
 * center, a start angle of 135 begins along the line running northwest,
 * and a start angle of -90 begins along the line running south.  The sweep
 * parameter indicates the extent of the arc and is also measured in
 * degrees counterclockwise.  A sweep angle of 90 defines a quarter circle
 * extending counterclockwise from the start angle, and a sweep angle of
 * -180 defines a semicircle extending clockwise.
 */

void drawArc(const GRectangle & bounds, double start, double sweep);
void drawArc(double x, double y, double width, double height,
                                 double start, double sweep);

/*
 * Function: fillArc
 * Usage: fillArc(bounds, start, sweep);
 *        fillArc(x, y, width, height, start, sweep);
 * --------------------------------------------------
 * Fills a wedge-shaped area of an elliptical arc.  The parameters are
 * interpreted in the same way as those for drawArc.
 */

void fillArc(const GRectangle & bounds, double start, double sweep);
void fillArc(double x, double y, double width, double height,
                                 double start, double sweep);

/*
 * Function: drawImage
 * Usage: drawImage(filename, pt);
 *        drawImage(filename, x, y);
 *        drawImage(filename, bounds);
 *        drawImage(filename, x, y, width, height);
 * ------------------------------------------------
 * Draws the image from the specified file with its upper left corner at
 * the specified point.  The forms of the call that include the bounds
 * scale the image so that it fits inside the specified rectangle.
 */

void drawImage(std::string filename, const GPoint & pt);
void drawImage(std::string filename, double x, double y);
void drawImage(std::string filename, const GRectangle & bounds);
void drawImage(std::string filename, double x, double y,
                                     double width, double height);

/*
 * Function: getImageBounds
 * Usage: GRectangle bounds = getImageBounds(filename);
 * ----------------------------------------------------
 * Returns the bounds of the image contained in the specified file.  Only
 * the width and height components of the rectangle are of interest; the x
 * and y components are always 0.
 */

GRectangle getImageBounds(std::string filename);

/*
 * Function: drawLine
 * Usage: drawLine(p0, p1);
 *        drawLine(x0, y0, x1, y1);
 * --------------------------------
 * Draws a line connecting the specified points.
 */

void drawLine(const GPoint & p0, const GPoint & p1);
void drawLine(double x0, double y0, double x1, double y1);

/*
 * Function: drawPolarLine
 * Usage: GPoint p1 = drawPolarLine(p0, r, theta);
 *        GPoint p1 = drawPolarLine(x0, y0, r, theta);
 * ---------------------------------------------------
 * Draws a line of length r in the direction theta from the initial point. 
 * The angle theta is measured in degrees counterclockwise from the +x
 * axis.  The function returns the end point of the line.
 */

GPoint drawPolarLine(const GPoint & p0, double r, double theta);
GPoint drawPolarLine(double x0, double y0, double r, double theta);

/*
 * Function: drawOval
 * Usage: drawOval(bounds);
 *        drawOval(x, y, width, height);
 * -------------------------------------
 * Draws the frame of a oval with the specified bounds.
 */

void drawOval(const GRectangle & bounds);
void drawOval(double x, double y, double width, double height);

/*
 * Function: fillOval
 * Usage: fillOval(bounds);
 *        fillOval(x, y, width, height);
 * -------------------------------------
 * Fills the frame of a oval with the specified bounds.
 */

void fillOval(const GRectangle & bounds);
void fillOval(double x, double y, double width, double height);

/*
 * Function: drawRect
 * Usage: drawRect(bounds);
 *        drawRect(x, y, width, height);
 * -------------------------------------
 * Draws the frame of a rectangle with the specified bounds.
 */

void drawRect(const GRectangle & bounds);
void drawRect(double x, double y, double width, double height);

/*
 * Function: fillRect
 * Usage: fillRect(bounds);
 *        fillRect(x, y, width, height);
 * -------------------------------------
 * Fills the frame of a rectangle with the specified bounds.
 */

void fillRect(const GRectangle & bounds);
void fillRect(double x, double y, double width, double height);

/*
 * Function: drawPolygon
 * Usage: drawPolygon(polygon);
 *        drawPolygon(polygon, pt);
 *        drawPolygon(polygon, x, y);
 * ----------------------------------
 * Draws the outline of the specified polygon.  The optional pt or x and y
 * parameters shift the origin of the polygon to the specified point.
 */

void drawPolygon(const Vector<GPoint> & polygon);
void drawPolygon(const Vector<GPoint> & polygon, const GPoint & pt);
void drawPolygon(const Vector<GPoint> & polygon, double x, double y);

/*
 * Function: fillPolygon
 * Usage: fillPolygon(polygon);
 *        fillPolygon(polygon, pt);
 *        fillPolygon(polygon, x, y);
 * ----------------------------------
 * Fills the frame of the specified polygon.  The optional pt or x and y
 * parameters shift the origin of the polygon to the specified point.
 */

void fillPolygon(const Vector<GPoint> & polygon);
void fillPolygon(const Vector<GPoint> & polygon, const GPoint & pt);
void fillPolygon(const Vector<GPoint> & polygon, double x, double y);

/*
 * Function: drawString
 * Usage: drawString(str, pt);
 *        drawString(str, x, y);
 * -----------------------------
 * Draws the string str so that its baseline origin appears at the
 * specified point.  The text appears in the current font and color.
 */

void drawString(std::string str, const GPoint & pt);
void drawString(std::string str, double x, double y);

/*
 * Function: getStringWidth
 * Usage: double width = getStringWidth(str);
 * ------------------------------------------
 * Returns the width of the string str when displayed in the current font.
 */

double getStringWidth(std::string str);

/*
 * Function: setFont
 * Usage: setFont(font);
 * ---------------------
 * Sets a new font.  The font parameter is a string in the form
 * family-style-size.  In this string, family is the name of the font
 * family; style is either missing (indicating a plain font) or one of the
 * strings Bold, Italic, or BoldItalic; and size is an integer indicating
 * the point size.  If any of these components is specified as an asterisk,
 * the existing value is retained.  The font parameter can also be a
 * sequence of such specifications separated by semicolons, in which the
 * first available font on the system is used.
 */

void setFont(std::string font);

/*
 * Function: getFont
 * Usage: string font = getFont();
 * -------------------------------
