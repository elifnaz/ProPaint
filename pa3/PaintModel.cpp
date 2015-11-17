#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>
#include <iostream>

PaintModel::PaintModel()
{
    pen = *wxBLACK_PEN;
    brush = *wxWHITE_BRUSH;

}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
	// TODO
    if (bitmap.IsOk())
    {
        dc.DrawBitmap(bitmap, 0, 0);
    }
    
    
    bool resetSelection = true;
    
    
    for (std::vector<std::shared_ptr<Shape>>::iterator it = mShapes.begin();
         it != mShapes.end(); ++it)
    {
        (*it)->Draw(dc);
        if (selectedShape == (*it))
        {
            resetSelection = false;
            
            if (showSelection)
                selectedShape->DrawSelection(dc);
        }
        
    }
    if (resetSelection && showSelection)
    {
        selectedShape.reset();
    }
    
    
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
	// TODO
    undo.clear();
    redo.clear();
    activeCommand.reset();
    mShapes.clear();
    pen = *wxBLACK_PEN;
    brush = *wxWHITE_BRUSH;
    selectedShape.reset();
    undoPen.clear();
    redoPen.clear();
    undoShape.clear();
    redoShape.clear();
    bitmap = wxBitmap();

}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
    shape->SetPenColor(GetPenColor());
    shape->SetBColor(GetBrushColor());
    shape->SetWidth(GetWidth());
    mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
	}
}

bool PaintModel::HasActiveCommand()
{
    if (activeCommand)
        return true;
    else
        return false;
}

void PaintModel::CreateCommand(CommandType type, const wxPoint &start)
{
    
    activeCommand = CommandFactory::Create(shared_from_this(), type, start);
}

void PaintModel::FinalizeCommand()
{
    activeCommand->Finalize(shared_from_this());
   
    
    
}

void PaintModel::UpdateCommand(const wxPoint &newPoint)
{
    
    
    (*activeCommand).Update(newPoint);
    
    
}

bool PaintModel::CanRedo()
{
    if (redo.empty())
        return false;
    else return true;
}

bool PaintModel::CanUndo()
{
    if (undo.empty())
        return false;
    else return true;
    
}

void PaintModel::Undo()
{    
    redo.push_back(undo.back());
    undo.pop_back();
    
}

void PaintModel::Redo()
{
    undo.push_back(redo.back());
    redo.pop_back();
    
}

int PaintModel::GetWidth()
{
    return pen.GetWidth();
}

wxColour PaintModel::GetPenColor()
{
    
    return pen.GetColour();
}

wxColour PaintModel::GetBrushColor()
{
    
    return brush.GetColour();

}


void PaintModel::SetBColor(wxColour color)
{

    
    if (selectedShape)
    {
    
        selectedShape->SetBColor(color);

    }
    

    brush.SetColour(color);
    
}


void PaintModel::SetPenColor(wxColour color)
{
    
    if (selectedShape)
    {
        selectedShape->SetPenColor(color);
        
    }
    pen.SetColour(color);
    
    
}

void PaintModel::SetWidth(int width)
{
    if (selectedShape)
    {
        selectedShape->SetWidth(width);
        
    }
    pen.SetWidth(width);
    

    
}

wxPen PaintModel::GetPen() const
{
    return pen;
}

wxBrush PaintModel::GetBrush() const
{
    return brush;
}

bool PaintModel::SelectShape(wxPoint pt)
{
    int size = static_cast<int>(mShapes.size()) -1;
    for (int i = size; i > -1; i--)
    {
        if (mShapes.at(i) ->Intersects(pt))
        {
            selectedShape = mShapes.at(i);
            return true;
        }
        selectedShape.reset();
    }
    return false;
    
}

void PaintModel::Export(wxString fileName, wxSize bitSize)
{
    wxString ext = fileName.substr(fileName.size() - 4, fileName.size() - 1);
    
    wxBitmapType type;
    
    if (ext == ".bmp")
        type = wxBITMAP_TYPE_BMP;
    else if (ext == ".png")
        type = wxBITMAP_TYPE_PNG;
    else if (ext == ".jpg")
        type = wxBITMAP_TYPE_JPEG;
    else if (ext == "jpeg")
        type = wxBITMAP_TYPE_JPEG;
    
    wxBitmap bitmap;
    // Create the bitmap of the specified wxSize
    bitmap.Create(bitSize);
    // Create a memory DC to draw to the bitmap
    wxMemoryDC dc(bitmap);
    // Clear the background color
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    // Draw all the shapes (make sure not the selection!)
    DrawShapes(dc, false);
    // Write the bitmap with the specified file name and wxBitmapType
    bitmap.SaveFile(fileName, type);
    
}

void PaintModel::Import(const wxString &fileName)
{
    New();
    wxString ext = fileName.substr(fileName.size() - 4, fileName.size() - 1);
    
    wxBitmapType type;
    
    if (ext == ".bmp")
        type = wxBITMAP_TYPE_BMP;
    else if (ext == ".png")
        type = wxBITMAP_TYPE_PNG;
    else if (ext == ".jpg")
        type = wxBITMAP_TYPE_JPEG;
    else if (ext == "jpeg")
        type = wxBITMAP_TYPE_JPEG;
    

    bitmap.LoadFile(fileName, type);
    
}

