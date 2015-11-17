#pragma once
#include <memory>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);
    
    bool HasActiveCommand();

    void CreateCommand(CommandType type, const wxPoint &start);
    
    void UpdateCommand(const wxPoint &newPoint);
    
    void FinalizeCommand();
    
    bool CanUndo();
    
    bool CanRedo();
    
    void Undo();
    
    void Redo();
    
    int GetWidth();
    
    wxColour GetPenColor();
    
    wxColour GetBrushColor();
    
    void SetWidth(int width);

    void SetPenColor(wxColour color);
    
    void SetBColor(wxColour color);
    
    wxPen GetPen() const;
    
    wxBrush GetBrush() const;
    
    bool SelectShape(wxPoint pt);
    
    void Export(wxString fileName, wxSize bitSize);

    void Import(const wxString &fileName);
    
    std::shared_ptr<Command> & GetActiveCommand()
    {
        return activeCommand;
    }
    
    std::vector<std::shared_ptr<Shape>> & GetShapes()
    {
        return mShapes;
    }
    std::shared_ptr<Shape> & GetSelectedShape()
    {
        return selectedShape;
    }

    std::vector<std::shared_ptr<Command>> undo;
    std::vector<std::shared_ptr<Command>> redo;
    std::vector<wxPen> undoPen;
    std::vector<wxPen> redoPen;
    std::vector<wxBrush> undoBrush;
    std::vector<wxBrush> redoBrush;
    std::vector<std::shared_ptr<Shape>> undoShape;
    std::vector<std::shared_ptr<Shape>> redoShape;
    

private:
	// Vector of all the shapes in the model
    
    wxPen pen;
    wxBrush brush;
    wxBitmap bitmap;
    std::shared_ptr<Command> activeCommand;
    std::shared_ptr<Shape> selectedShape;
    std::vector<std::shared_ptr<Shape>> mShapes;

    
};
