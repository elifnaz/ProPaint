#include "Shape.h"
#include <iostream>

Shape::Shape(const wxPoint& start)
	:mStartPoint(start)
	,mEndPoint(start)
	,mTopLeft(start)
	,mBotRight(start)
{
    pen = *wxBLACK_PEN;
    brush = *wxWHITE_BRUSH;
    
}

// Tests whether the provided point intersects
// with this shape
bool Shape::Intersects(const wxPoint& point) const
{
	wxPoint topleft;
	wxPoint botright;
	GetBounds(topleft, botright);
	if (point.x >= topleft.x && point.x <= botright.x &&
		point.y >= topleft.y && point.y <= botright.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Update shape with new provided point
void Shape::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;

	// For most shapes, we only have two points - start and end
	// So we can figure out the top left/bottom right bounds
	// based on this.
	mTopLeft.x = std::min(mStartPoint.x, mEndPoint.x);
	mTopLeft.y = std::min(mStartPoint.y, mEndPoint.y);
	mBotRight.x = std::max(mStartPoint.x, mEndPoint.x);
	mBotRight.y = std::max(mStartPoint.y, mEndPoint.y);
}

void Shape::Finalize()
{
	// Default finalize doesn't do anything
}

void Shape::GetBounds(wxPoint& topLeft, wxPoint& botRight) const
{
    topLeft = mTopLeft + mOffset;
    botRight = mBotRight + mOffset;
}



int Shape::GetWidth()
{
    return pen.GetWidth();
}

wxColour Shape::GetPenColor()
{
    
    return pen.GetColour();
}

wxColour Shape::GetBrushColor()
{
    
    return brush.GetColour();
    
}


void Shape::SetBColor(wxColour color)
{
    brush.SetColour(color);

}


void Shape::SetPenColor(wxColour color)
{
    
    pen.SetColour(color);
    
}


void Shape::SetWidth(int width)
{
    
    pen.SetWidth(width);
}

wxPen Shape::GetPen() const
{
    return pen;
}

wxBrush Shape::GetBrush() const
{
    return brush;
}

void Shape::DrawSelection(wxDC& dc)
{
    wxPen dottedPen = *wxBLACK_DASHED_PEN;
    wxBrush dottedB = *wxTRANSPARENT_BRUSH;
    dc.SetPen(dottedPen);
    dc.SetBrush(dottedB);
    
    wxPoint x;
    wxPoint y;
    GetBounds(x, y);

    wxRect rect(x - wxPoint(5, 5), y + wxPoint(5, 5));
    dc.DrawRectangle(rect);
}


RectShape::RectShape(const wxPoint& start) : Shape(start)
{
    
}

void RectShape::Draw(wxDC& dc) const{
    
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());
    
    wxPoint a, b;
    GetBounds(a, b);

    wxRect re(a, b);
    dc.DrawRectangle(re);
    
}

EllipseShape::EllipseShape(const wxPoint& start) : Shape(start)
{
    
}

void EllipseShape::Draw(wxDC& dc) const{
    
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());
    
    wxPoint top, bot;
    GetBounds(top, bot);
    
    wxRect rect(top, bot);
    dc.DrawEllipse(rect);
    
}


LineShape::LineShape(const wxPoint& start) : Shape(start)
{
    
}

void LineShape::Draw(wxDC& dc) const{
    
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());

    dc.DrawLine(mStartPoint + mOffset, mEndPoint + mOffset);
    
}


PencilShape::PencilShape(const wxPoint& start) : Shape(start)
{
    points.push_back(start);
}

void PencilShape::Draw(wxDC& dc) const{
    
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());
    
    
    const wxPoint* ptr = &(points.front()); // or points.data()
    if(points.size() == 1)
        dc.DrawPoint(*ptr);
    else
    {
        if (mOffset == wxPoint(0, 0))
        {
            
            dc.DrawLines(static_cast<int>(points.size()), ptr);
            
            
        }
        else
        
        
            dc.DrawLines(static_cast<int>(points.size()), ptr, mOffset.x, mOffset.y);
        
    }
    
}

void PencilShape::Update(const wxPoint &newPoint)
{
    Shape::Update(newPoint);
    points.push_back(newPoint);

}

void PencilShape::Finalize()
{
    int top, left, bot, right;
    std::vector<wxPoint>::iterator it = points.begin();
    top = it->y;
    bot = it->y;
    left = it->x;
    right = it->x;

    ++it;
    for (; it != points.end(); ++it)
    {
        if (it->x < left)
            left = it->x;
        if (it->x > right)
            right = it->x;
        if (it->y < top)
            top = it->y;
        if (it->y > bot)
            bot = it->y;
        
    }
    
    mTopLeft.x = left;
    mTopLeft.y = top;
    mBotRight.x = right;
    mBotRight.y = bot;
    
}

void Shape::UpdateOffset(const wxPoint &offset)
{
    mOffset.x = offset.x - mStartPoint.x;
    mOffset.y = offset.y - mStartPoint.y ;
    
    
}

