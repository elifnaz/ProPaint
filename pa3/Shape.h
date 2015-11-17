#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
	virtual ~Shape() { }
    
    int GetWidth();
    
    wxColour GetPenColor();
    
    wxColour GetBrushColor();
    
    void SetWidth(int width);
    
    void SetPenColor(wxColour color);
    
    void SetBColor(wxColour color);
    
    void DrawSelection(wxDC& dc);
    
    wxPen GetPen() const;
    
    wxBrush GetBrush() const;
    
    void UpdateOffset(const wxPoint &offset);
    
    wxPoint mOffset;
    
    std::vector<wxBrush> undoBrush;
    std::vector<wxBrush> redoBrush;
    std::vector<wxPen> undoPen;
    std::vector<wxPen> redoPen;

protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;
    
    wxPen pen;
    wxBrush brush;
   
};



class RectShape : public Shape
{
public:
    
    RectShape(const wxPoint& start);    
    void Draw(wxDC& dc) const override;
    
};


class EllipseShape : public Shape
{
public:
    
    EllipseShape(const wxPoint& start);
    
    void Draw(wxDC& dc) const override;
    
};


class LineShape : public Shape
{
public:
    
    LineShape(const wxPoint& start);
    
    void Draw(wxDC& dc) const override;
    
};


class PencilShape : public Shape
{
public:
    
    PencilShape(const wxPoint& start);
    
    void Draw(wxDC& dc) const override;
    void Update(const wxPoint& newPoint) override;
    void Finalize() override;
    
    std::vector<wxPoint> points;
    
};


