/*
 // Copyright (c) 2021-2022 Timothy Schoen
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#pragma once

#include <JuceHeader.h>

#include "Box.h"
#include "BoxEditor.h"
#include "Pd/PdPatch.h"
#include "PluginProcessor.h"

extern JUCEApplicationBase* juce_CreateApplication();


struct GraphArea;
class Edge;
class PlugDataPluginEditor;
class PlugDataPluginProcessor;
class Canvas : public Component, public MultiComponentDragger<Box>, public Value::Listener
{
   public:
    Canvas(PlugDataPluginEditor& parent, const pd::Patch& patch, bool isGraph = false, bool isGraphChild = false);

    ~Canvas() override;

    PlugDataPluginEditor& main;
    PlugDataAudioProcessor* pd;

    void paintOverChildren(Graphics&) override;

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;

    void synchronise(bool updatePosition = true);

    bool keyPressed(const KeyPress& key) override;

    void copySelection();
    void removeSelection();
    void pasteSelection();
    void duplicateSelection();
    
    void valueChanged(Value& v) override;

    void checkBounds();

    void paint(Graphics& g) override;

    void deselectAll() override;

    void focusGained(FocusChangeType cause) override;

    void undo();
    void redo();

    void dragCallback(int dx, int dy) override;

    void findDrawables(Graphics& g);

    Viewport* viewport = nullptr;

    bool connectingWithDrag = false;
    SafePointer<Edge> connectingEdge;

    pd::Patch patch;

    OwnedArray<Box> boxes;
    OwnedArray<Connection> connections;
    
    Value locked;
    Value connectionStyle;

    bool isGraph = false;
    bool isGraphChild = false;

    std::unique_ptr<GraphArea> graphArea;

    Point<int> zeroPosition = {0, 0};
    Point<int> lastMousePos;

    SuggestionBox suggestor;

   private:
    SafePointer<TabbedComponent> tabbar;

    LassoComponent<Box*> lasso;
    PopupMenu popupMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Canvas)
};

// Graph bounds component
struct GraphArea : public Component, public ComponentDragger
{
    ResizableBorderComponent resizer;
    Canvas* canvas;

    Rectangle<int> startRect;

    explicit GraphArea(Canvas* parent) : resizer(this, nullptr), canvas(parent)
    {
        addAndMakeVisible(resizer);
    }

    void paint(Graphics& g) override
    {
        g.setColour(findColour(Slider::thumbColourId));
        g.drawRect(getLocalBounds());

        g.setColour(findColour(Slider::thumbColourId).darker(0.8f));
        g.drawRect(getLocalBounds().reduced(6));
    }

    bool hitTest(int x, int y) override
    {
        return !getLocalBounds().reduced(8).contains(Point<int>{x, y});
    }

    void mouseMove(const MouseEvent& e) override
    {
        setMouseCursor(MouseCursor::UpDownLeftRightResizeCursor);
    }

    void mouseDown(const MouseEvent& e) override
    {
        startDraggingComponent(this, e);
    }

    void mouseDrag(const MouseEvent& e) override
    {
        dragComponent(this, e, nullptr);
    }

    void mouseUp(const MouseEvent& e) override
    {
        updatePosition();
        repaint();
    }

    void resized() override
    {
        updatePosition();
        resizer.setBounds(getLocalBounds());
        repaint();
    }

    void updatePosition()
    {
        t_canvas* cnv = canvas->patch.getPointer();
        // Lock first?
        if (cnv)
        {
            cnv->gl_pixwidth = getWidth() / pd::Patch::zoom;
            cnv->gl_pixheight = getHeight() / pd::Patch::zoom;
            cnv->gl_xmargin = (getX() - 4) / pd::Patch::zoom;
            cnv->gl_ymargin = (getY() - 4) / pd::Patch::zoom;
        }
    }
};
