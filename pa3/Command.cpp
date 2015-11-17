#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"
#include <iostream>

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;
    std::shared_ptr<Shape> sharedShape;
    
	// TODO: Actually create a command based on the type passed in
    
    switch (type) {
        case CM_DrawRect:
            sharedShape = std::make_shared<RectShape>(start);
            retVal = std::make_shared<DrawCommand> (start, sharedShape);
            model->AddShape(sharedShape);
            break;
        case CM_DrawEllipse:
            sharedShape = std::make_shared<EllipseShape>(start);
            retVal = std::make_shared<DrawCommand> (start, sharedShape);
            model->AddShape(sharedShape);
            break;
        case CM_DrawLine:
            sharedShape = std::make_shared<LineShape>(start);
            retVal = std::make_shared<DrawCommand> (start, sharedShape);
            model->AddShape(sharedShape);
            break;
        case CM_DrawPencil:
            sharedShape = std::make_shared<PencilShape>(start);
            retVal = std::make_shared<DrawCommand> (start, sharedShape);
            model->AddShape(sharedShape);
            break;
        case CM_SetPen:
            retVal = std::make_shared<SetPenCommand> (start, sharedShape);
            break;
        case CM_SetBrush:
            retVal = std::make_shared<SetBrushCommand> (start, sharedShape);
            break;
        case CM_Delete:
            retVal = std::make_shared<DeleteCommand> (start, sharedShape);
            break;
        case CM_Move:
            sharedShape = model->GetSelectedShape();
            retVal = std::make_shared<MoveCommand> (start, sharedShape);
            break;
        default:
            break;
    }
    
	return retVal;
}


DrawCommand::DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command::Command(start, shape)
{

}

void DrawCommand::Update(const wxPoint &newPoint)
{
    
    Command::Update(newPoint);
    mShape->Update(newPoint);
    
}

void DrawCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
    model->undo.push_back(model->GetActiveCommand());
    model->GetActiveCommand().reset();
}
void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
   
    model->GetShapes().pop_back();
    model->Undo();
    
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->GetShapes().push_back((*(model->redo.back())).getShape());
    model->Redo();

}

SetPenCommand::SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command::Command(start, shape)
{

}


void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    model->undo.push_back(model->GetActiveCommand());
    model->GetActiveCommand().reset();
    
    if (model->GetSelectedShape())
    {
        model->GetSelectedShape()->undoPen.push_back(model->GetPen());
        shape = model->GetSelectedShape();
        
    }
}
void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->redoPen.push_back(model->GetPen());

    if (shape)
    {
        
        auto iter = model->GetShapes().begin();
        
        for (; iter!= model->GetShapes().end(); ++iter){
            if (*iter == shape)
            {
                (*iter)->redoPen.push_back( (*iter)->GetPen());
                (*iter)->SetPenColor((*iter)->undoPen.back().GetColour());

                (*iter)->SetWidth((*iter)->undoPen.back().GetWidth());
                (*iter)->undoPen.pop_back();
            }
        }
    }

    //model->SetWidth(model->undoPen.back().GetWidth());
    //model->SetPenColor(model->undoPen.back().GetColour());
    model->undoPen.pop_back();
    model->Undo();
    
    
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->undoPen.push_back(model->GetPen());
    
    if (shape)
    {
        auto iter = model->GetShapes().begin();
        
        for (; iter!= model->GetShapes().end(); ++iter){
            if (*iter == shape)
            {

                (*iter)->undoPen.push_back( (*iter)->GetPen());
                (*iter)->SetPenColor((*iter)->redoPen.back().GetColour());
                (*iter)->SetWidth((*iter)->redoPen.back().GetWidth());
                (*iter)->redoPen.pop_back();
            }
        }
    }

//    model->SetWidth(model->redoPen.back().GetWidth());
  //  model->SetPenColor(model->redoPen.back().GetColour());
    model->redoPen.pop_back();
    model->Redo();
   
}


SetBrushCommand::SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command::Command(start, shape)
{
    
}


void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    model->undo.push_back(model->GetActiveCommand());
    model->GetActiveCommand().reset();
    
    if (model->GetSelectedShape())
    {
        model->GetSelectedShape()->undoBrush.push_back(model->GetBrush());
        shape = model->GetSelectedShape();

    }

}
void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
    
    model->redoBrush.push_back(model->GetBrush());
    if (shape)
    {
        auto iter = model->GetShapes().begin();
        
        for (; iter!= model->GetShapes().end(); ++iter){
            if (*iter == shape)
            {
                (*iter)->redoBrush.push_back( (*iter)->GetBrush());
                (*iter)->SetBColor((*iter)->undoBrush.back().GetColour());
                (*iter)->undoBrush.pop_back();
            }
        }
    }
 //   model->SetBColor(model->undoBrush.back().GetColour());

    model->undoBrush.pop_back();
    model->Undo();
    
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->undoBrush.push_back(model->GetBrush());
    if (shape)
    {
        auto iter = model->GetShapes().begin();
        
        for (; iter!= model->GetShapes().end(); ++iter){
            if (*iter == shape)
            {
                (*iter)->undoBrush.push_back( (*iter)->GetBrush());
                (*iter)->SetBColor((*iter)->redoBrush.back().GetColour());
                (*iter)->redoBrush.pop_back();
            }
        }
    }
    
  //  model->SetBColor(model->redoBrush.back().GetColour());

    model->redoBrush.pop_back();
    model->Redo();

}

DeleteCommand::DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command::Command(start, shape)
{
    
}


void DeleteCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    model->undoShape.push_back(model->GetSelectedShape());
    model->RemoveShape(model->GetSelectedShape());
    model->GetSelectedShape().reset();
    
    model->undo.push_back(model->GetActiveCommand());
    model->GetActiveCommand().reset();
}
void DeleteCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->AddShape(model->undoShape.back());
    model->redoShape.push_back(model->undoShape.back());
    model->undoShape.pop_back();
    model->Undo();
}

void DeleteCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(model->redoShape.back());
    model->undoShape.push_back(model->redoShape.back());
    model->redoShape.pop_back();
    model->Redo();
    
}


MoveCommand::MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command::Command(start, shape)
{
    
}

void MoveCommand::Update(const wxPoint &newPoint)
{
    mShape->UpdateOffset(newPoint);
    
}


void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    model->GetActiveCommand().reset();
}
void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->Undo();
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->Redo();
    
}


