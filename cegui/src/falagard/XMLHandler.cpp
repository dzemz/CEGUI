/***********************************************************************
    created:    Fri Jun 17 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/ColourRect.h"

#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/WidgetComponent.h"
#include "CEGUI/falagard/TextComponent.h"
#include "CEGUI/falagard/FrameComponent.h"
#include "CEGUI/falagard/NamedArea.h"
#include "CEGUI/falagard/PropertyDefinition.h"
#include "CEGUI/falagard/PropertyLinkDefinition.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Animation_xmlHandler.h"
#include "CEGUI/AnimationManager.h"

#include "CEGUI/widgets/Thumb.h"
#include "CEGUI/widgets/TabControl.h"
#include "CEGUI/widgets/Spinner.h"
#include "CEGUI/widgets/ItemListBase.h"
#include "CEGUI/widgets/ListHeaderSegment.h"
#include "CEGUI/widgets/MultiColumnList.h"

#include <sstream>

// Start of CEGUI namespace section
namespace CEGUI
{
    // note: The assets' versions aren't usually the same as CEGUI version, they are versioned from version 1 onwards!
    const String Falagard_xmlHandler::NativeVersion("7");

    ////////////////////////////////////////////////////////////////////////////////
    // element names
    const String Falagard_xmlHandler::FalagardElement("Falagard");
    const String Falagard_xmlHandler::WidgetLookElement("WidgetLook");
    const String Falagard_xmlHandler::ChildElement("Child");
    const String Falagard_xmlHandler::ImagerySectionElement("ImagerySection");
    const String Falagard_xmlHandler::StateImageryElement("StateImagery");
    const String Falagard_xmlHandler::LayerElement("Layer");
    const String Falagard_xmlHandler::SectionElement("Section");
    const String Falagard_xmlHandler::ImageryComponentElement("ImageryComponent");
    const String Falagard_xmlHandler::TextComponentElement("TextComponent");
    const String Falagard_xmlHandler::FrameComponentElement("FrameComponent");
    const String Falagard_xmlHandler::AreaElement("Area");
    const String Falagard_xmlHandler::ImageElement("Image");
    const String Falagard_xmlHandler::ColoursElement("Colours");
    const String Falagard_xmlHandler::VertFormatElement("VertFormat");
    const String Falagard_xmlHandler::HorzFormatElement("HorzFormat");
    const String Falagard_xmlHandler::VertAlignmentElement("VertAlignment");
    const String Falagard_xmlHandler::HorzAlignmentElement("HorzAlignment");
    const String Falagard_xmlHandler::PropertyElement("Property");
    const String Falagard_xmlHandler::DimElement("Dim");
    const String Falagard_xmlHandler::UnifiedDimElement("UnifiedDim");
    const String Falagard_xmlHandler::AbsoluteDimElement("AbsoluteDim");
    const String Falagard_xmlHandler::ImageDimElement("ImageDim");
    const String Falagard_xmlHandler::ImagePropertyDimElement("ImagePropertyDim");
    const String Falagard_xmlHandler::WidgetDimElement("WidgetDim");
    const String Falagard_xmlHandler::FontDimElement("FontDim");
    const String Falagard_xmlHandler::PropertyDimElement("PropertyDim");
    const String Falagard_xmlHandler::TextElement("Text");
    const String Falagard_xmlHandler::ColourPropertyElement("ColourProperty");
    const String Falagard_xmlHandler::ColourRectPropertyElement("ColourRectProperty");
    const String Falagard_xmlHandler::NamedAreaElement("NamedArea");
    const String Falagard_xmlHandler::PropertyDefinitionElement("PropertyDefinition");
    const String Falagard_xmlHandler::PropertyLinkDefinitionElement("PropertyLinkDefinition");
    const String Falagard_xmlHandler::PropertyLinkTargetElement("PropertyLinkTarget");
    const String Falagard_xmlHandler::OperatorDimElement("OperatorDim");
    const String Falagard_xmlHandler::VertFormatPropertyElement("VertFormatProperty");
    const String Falagard_xmlHandler::HorzFormatPropertyElement("HorzFormatProperty");
    const String Falagard_xmlHandler::AreaPropertyElement("AreaProperty");
    const String Falagard_xmlHandler::ImagePropertyElement("ImageProperty");
    const String Falagard_xmlHandler::TextPropertyElement("TextProperty");
    const String Falagard_xmlHandler::FontPropertyElement("FontProperty");
    const String Falagard_xmlHandler::ColourElement("Colour");
    const String Falagard_xmlHandler::EventLinkDefinitionElement("EventLinkDefinition");
    const String Falagard_xmlHandler::EventLinkTargetElement("EventLinkTarget");
    const String Falagard_xmlHandler::NamedAreaSourceElement("NamedAreaSource");
    const String Falagard_xmlHandler::EventActionElement("EventAction");
    // attribute names
    const String Falagard_xmlHandler::VersionAttribute("version");
    const String Falagard_xmlHandler::TopLeftAttribute("topLeft");
    const String Falagard_xmlHandler::TopRightAttribute("topRight");
    const String Falagard_xmlHandler::BottomLeftAttribute("bottomLeft");
    const String Falagard_xmlHandler::BottomRightAttribute("bottomRight");
    const String Falagard_xmlHandler::TypeAttribute("type");
    const String Falagard_xmlHandler::NameAttribute("name");
    const String Falagard_xmlHandler::PriorityAttribute("priority");
    const String Falagard_xmlHandler::SectionNameAttribute("section");
    const String Falagard_xmlHandler::NameSuffixAttribute("nameSuffix");
    const String Falagard_xmlHandler::RendererAttribute("renderer");
    const String Falagard_xmlHandler::LookAttribute("look");
    const String Falagard_xmlHandler::ScaleAttribute("scale");
    const String Falagard_xmlHandler::OffsetAttribute("offset");
    const String Falagard_xmlHandler::ValueAttribute("value");
    const String Falagard_xmlHandler::DimensionAttribute("dimension");
    const String Falagard_xmlHandler::WidgetAttribute("widget");
    const String Falagard_xmlHandler::StringAttribute("string");
    const String Falagard_xmlHandler::FontAttribute("font");
    const String Falagard_xmlHandler::InitialValueAttribute("initialValue");
    const String Falagard_xmlHandler::ClippedAttribute("clipped");
    const String Falagard_xmlHandler::OperatorAttribute("op");
    const String Falagard_xmlHandler::PaddingAttribute("padding");
    const String Falagard_xmlHandler::LayoutOnWriteAttribute("layoutOnWrite");
    const String Falagard_xmlHandler::RedrawOnWriteAttribute("redrawOnWrite");
    const String Falagard_xmlHandler::TargetPropertyAttribute("targetProperty");
    const String Falagard_xmlHandler::ControlPropertyAttribute("controlProperty");
    const String Falagard_xmlHandler::ColourAttribute("colour");
    const String Falagard_xmlHandler::PropertyAttribute("property");
    const String Falagard_xmlHandler::ControlValueAttribute("controlValue");
    const String Falagard_xmlHandler::ControlWidgetAttribute("controlWidget");
    const String Falagard_xmlHandler::HelpStringAttribute("help");
    const String Falagard_xmlHandler::EventAttribute("event");
    const String Falagard_xmlHandler::InheritsAttribute("inherits");
    const String Falagard_xmlHandler::AutoWindowAttribute("autoWindow");
    const String Falagard_xmlHandler::FireEventAttribute("fireEvent");
    const String Falagard_xmlHandler::ActionAttribute("action");
    const String Falagard_xmlHandler::ComponentAttribute("component");

    // Default values
    const String Falagard_xmlHandler::PropertyDefinitionHelpDefaultValue("Falagard custom property definition - "
                                                                         "gets/sets a named user string.");
    const String Falagard_xmlHandler::PropertyLinkDefinitionHelpDefaultValue("Falagard property link definition - links a "
                                                                             "property on this window to properties "
                                                                             "defined on one or more child windows, or "
                                                                             "the parent window.");

    // Specific attribute values
    const String Falagard_xmlHandler::GenericDataType("Generic");
    const String Falagard_xmlHandler::ParentIdentifier("__parent__");

    ////////////////////////////////////////////////////////////////////////////////


    Falagard_xmlHandler::Falagard_xmlHandler(WidgetLookManager* mgr) :
        d_manager(mgr),
        d_widgetlook(0),
        d_childcomponent(0),
        d_imagerysection(0),
        d_stateimagery(0),
        d_layer(0),
        d_section(0),
        d_imagerycomponent(0),
        d_area(0),
        d_textcomponent(0),
        d_namedArea(0),
        d_framecomponent(0),
        d_propertyLink(0),
        d_eventLink(0)
    {
        // register element start handlers
        registerElementStartHandler(FalagardElement, &Falagard_xmlHandler::elementFalagardStart);
        registerElementStartHandler(WidgetLookElement, &Falagard_xmlHandler::elementWidgetLookStart);
        registerElementStartHandler(ChildElement, &Falagard_xmlHandler::elementChildStart);
        registerElementStartHandler(ImagerySectionElement, &Falagard_xmlHandler::elementImagerySectionStart);
        registerElementStartHandler(StateImageryElement, &Falagard_xmlHandler::elementStateImageryStart);
        registerElementStartHandler(LayerElement, &Falagard_xmlHandler::elementLayerStart);
        registerElementStartHandler(SectionElement, &Falagard_xmlHandler::elementSectionStart);
        registerElementStartHandler(ImageryComponentElement, &Falagard_xmlHandler::elementImageryComponentStart);
        registerElementStartHandler(TextComponentElement, &Falagard_xmlHandler::elementTextComponentStart);
        registerElementStartHandler(FrameComponentElement, &Falagard_xmlHandler::elementFrameComponentStart);
        registerElementStartHandler(AreaElement, &Falagard_xmlHandler::elementAreaStart);
        registerElementStartHandler(ImageElement, &Falagard_xmlHandler::elementImageStart);
        registerElementStartHandler(ColoursElement, &Falagard_xmlHandler::elementColoursStart);
        registerElementStartHandler(VertFormatElement, &Falagard_xmlHandler::elementVertFormatStart);
        registerElementStartHandler(HorzFormatElement, &Falagard_xmlHandler::elementHorzFormatStart);
        registerElementStartHandler(VertAlignmentElement, &Falagard_xmlHandler::elementVertAlignmentStart);
        registerElementStartHandler(HorzAlignmentElement, &Falagard_xmlHandler::elementHorzAlignmentStart);
        registerElementStartHandler(PropertyElement, &Falagard_xmlHandler::elementPropertyStart);
        registerElementStartHandler(DimElement, &Falagard_xmlHandler::elementDimStart);
        registerElementStartHandler(UnifiedDimElement, &Falagard_xmlHandler::elementUnifiedDimStart);
        registerElementStartHandler(AbsoluteDimElement, &Falagard_xmlHandler::elementAbsoluteDimStart);
        registerElementStartHandler(ImageDimElement, &Falagard_xmlHandler::elementImageDimStart);
        registerElementStartHandler(ImagePropertyDimElement, &Falagard_xmlHandler::elementImagePropertyDimStart);
        registerElementStartHandler(WidgetDimElement, &Falagard_xmlHandler::elementWidgetDimStart);
        registerElementStartHandler(FontDimElement, &Falagard_xmlHandler::elementFontDimStart);
        registerElementStartHandler(PropertyDimElement, &Falagard_xmlHandler::elementPropertyDimStart);
        registerElementStartHandler(TextElement, &Falagard_xmlHandler::elementTextStart);
        registerElementStartHandler(ColourPropertyElement, &Falagard_xmlHandler::elementColourRectPropertyStart);
        registerElementStartHandler(ColourRectPropertyElement, &Falagard_xmlHandler::elementColourRectPropertyStart);
        registerElementStartHandler(NamedAreaElement, &Falagard_xmlHandler::elementNamedAreaStart);
        registerElementStartHandler(PropertyDefinitionElement, &Falagard_xmlHandler::elementPropertyDefinitionStart);
        registerElementStartHandler(PropertyLinkDefinitionElement, &Falagard_xmlHandler::elementPropertyLinkDefinitionStart);
        registerElementStartHandler(OperatorDimElement, &Falagard_xmlHandler::elementOperatorDimStart);
        registerElementStartHandler(VertFormatPropertyElement, &Falagard_xmlHandler::elementVertFormatPropertyStart);
        registerElementStartHandler(HorzFormatPropertyElement, &Falagard_xmlHandler::elementHorzFormatPropertyStart);
        registerElementStartHandler(AreaPropertyElement, &Falagard_xmlHandler::elementAreaPropertyStart);
        registerElementStartHandler(ImagePropertyElement, &Falagard_xmlHandler::elementImagePropertyStart);
        registerElementStartHandler(TextPropertyElement, &Falagard_xmlHandler::elementTextPropertyStart);
        registerElementStartHandler(FontPropertyElement, &Falagard_xmlHandler::elementFontPropertyStart);
        registerElementStartHandler(ColourElement, &Falagard_xmlHandler::elementColourStart);
        registerElementStartHandler(PropertyLinkTargetElement, &Falagard_xmlHandler::elementPropertyLinkTargetStart);
        registerElementStartHandler(AnimationDefinitionHandler::ElementName, &Falagard_xmlHandler::elementAnimationDefinitionStart);
        registerElementStartHandler(EventLinkDefinitionElement, &Falagard_xmlHandler::elementEventLinkDefinitionStart);
        registerElementStartHandler(EventLinkTargetElement, &Falagard_xmlHandler::elementEventLinkTargetStart);
        registerElementStartHandler(NamedAreaSourceElement, &Falagard_xmlHandler::elementNamedAreaSourceStart);
        registerElementStartHandler(EventActionElement, &Falagard_xmlHandler::elementEventActionStart);

        // register element end handlers
        registerElementEndHandler(FalagardElement, &Falagard_xmlHandler::elementFalagardEnd);
        registerElementEndHandler(WidgetLookElement, &Falagard_xmlHandler::elementWidgetLookEnd);
        registerElementEndHandler(ChildElement, &Falagard_xmlHandler::elementChildEnd);
        registerElementEndHandler(ImagerySectionElement, &Falagard_xmlHandler::elementImagerySectionEnd);
        registerElementEndHandler(StateImageryElement, &Falagard_xmlHandler::elementStateImageryEnd);
        registerElementEndHandler(LayerElement, &Falagard_xmlHandler::elementLayerEnd);
        registerElementEndHandler(SectionElement, &Falagard_xmlHandler::elementSectionEnd);
        registerElementEndHandler(ImageryComponentElement, &Falagard_xmlHandler::elementImageryComponentEnd);
        registerElementEndHandler(TextComponentElement, &Falagard_xmlHandler::elementTextComponentEnd);
        registerElementEndHandler(FrameComponentElement, &Falagard_xmlHandler::elementFrameComponentEnd);
        registerElementEndHandler(AreaElement, &Falagard_xmlHandler::elementAreaEnd);
        registerElementEndHandler(UnifiedDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(AbsoluteDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(ImageDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(ImagePropertyDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(WidgetDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(FontDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(PropertyDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(OperatorDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(NamedAreaElement, &Falagard_xmlHandler::elementNamedAreaEnd);
        registerElementEndHandler(PropertyLinkDefinitionElement, &Falagard_xmlHandler::elementPropertyLinkDefinitionEnd);
        registerElementEndHandler(EventLinkDefinitionElement, &Falagard_xmlHandler::elementEventLinkDefinitionEnd);
    }

    Falagard_xmlHandler::~Falagard_xmlHandler()
    {}

    /*************************************************************************
        Handle an opening XML element tag.
    *************************************************************************/
    void Falagard_xmlHandler::elementStartLocal(const String& element, const XMLAttributes& attributes)
    {
        // find registered handler for this element.
        ElementStartHandlerMap::const_iterator iter = d_startHandlersMap.find(element);

        // if a handler existed
        if (iter != d_startHandlersMap.end())
        {
            // call the handler for this element
            (this->*(iter->second))(attributes);
        }
        // no handler existed
        else
        {
            Logger::getSingleton().logEvent("Falagard::xmlHandler::elementStart - The unknown XML element '" + element + "' was encountered while processing the look and feel file.", Errors);
        }
    }

    /*************************************************************************
        Handle a closing XML element tag
    *************************************************************************/
    void Falagard_xmlHandler::elementEndLocal(const String& element)
    {
        // find registered handler for this element.
        ElementEndHandlerMap::const_iterator iter = d_endHandlersMap.find(element);

        // if a handler existed
        if (iter != d_endHandlersMap.end())
            // call the handler for this element
            (this->*(iter->second))();
    }

    /*************************************************************************
        Convert a hex string "AARRGGBB" to type argb_t
    *************************************************************************/
    argb_t Falagard_xmlHandler::hexStringToARGB(const String& str)
    {
        argb_t val;
        std::istringstream s(str.c_str());
        s >> std::hex >> val;

        return val;
    }

    /*************************************************************************
        Assign a dimension to a ComponentArea depending upon the dimension's
        type.
    *************************************************************************/
    void Falagard_xmlHandler::assignAreaDimension(Dimension& dim)
    {
        if (d_area)
        {
            switch (dim.getDimensionType())
            {
            case DT_LEFT_EDGE:
            case DT_X_POSITION:
                d_area->d_left = dim;
                break;
            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                d_area->d_top = dim;
                break;
            case DT_RIGHT_EDGE:
            case DT_WIDTH:
                d_area->d_right_or_width = dim;
                break;
            case DT_BOTTOM_EDGE:
            case DT_HEIGHT:
                d_area->d_bottom_or_height = dim;
                break;
            default:
                CEGUI_THROW(InvalidRequestException(
                    "Invalid DimensionType specified for area component."));
            }
        }
    }

    /*************************************************************************
        Assign a ColourRect to the current element supporting such a thing
    *************************************************************************/
    void Falagard_xmlHandler::assignColours(const ColourRect& cols)
    {
        // need to decide what to apply colours to
        if (d_framecomponent)
        {
            d_framecomponent->setColours(cols);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setColours(cols);
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setColours(cols);
        }
        else if (d_imagerysection)
        {
            d_imagerysection->setMasterColours(cols);
        }
        else if (d_section)
        {
            d_section->setOverrideColours(cols);
            d_section->setUsingOverrideColours(true);
        }
    }

    /*************************************************************************
        Method that performs common handling for all *Dim elements.
    *************************************************************************/
    void Falagard_xmlHandler::doBaseDimStart(const BaseDim* dim)
    {
        BaseDim* cloned = dim->clone();
        d_dimStack.push_back(cloned);
    }

    /*************************************************************************
        Method that handles the opening Falagard XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFalagardStart(const XMLAttributes& attributes)
    {
        Logger::getSingleton().logEvent("===== Falagard 'root' element: look and feel parsing begins =====");

        const String version = attributes.getValueAsString(VersionAttribute, "unknown");

        if (version != NativeVersion)
        {
            CEGUI_THROW(InvalidRequestException(
                "You are attempting to load a looknfeel of version '" + version +
                "' but this CEGUI version is only meant to load looknfeels of "
                "version '" + NativeVersion + "'. Consider using the migrate.py "
                "script bundled with CEGUI Unified Editor to migrate your data."));
        }
    }

    /*************************************************************************
        Method that handles the opening WidgetLook XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementWidgetLookStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook == 0);
        d_widgetlook = new WidgetLookFeel(attributes.getValueAsString(NameAttribute),
                                          attributes.getValueAsString(InheritsAttribute));

        Logger::getSingleton().logEvent("---> Start of definition for widget look '" + d_widgetlook->getName() + "'.", Informative);
    }

    /*************************************************************************
        Method that handles the opening Child XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementChildStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent == 0);
        d_childcomponent = new WidgetComponent(
            attributes.getValueAsString(TypeAttribute),
            attributes.getValueAsString(NameSuffixAttribute),
            attributes.getValueAsString(RendererAttribute),
            attributes.getValueAsBool(AutoWindowAttribute, true));

        CEGUI_LOGINSANE("-----> Start of definition for child widget."
            " Type: " + d_childcomponent->getTargetType() +
            " Name: " + d_childcomponent->getWidgetName() +
            " Auto: " + (d_childcomponent->isAutoWindow() ? "Yes" : "No"));
    }

    /*************************************************************************
        Method that handles the opening ImagerySection XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagerySectionStart(const XMLAttributes& attributes)
    {
        assert(d_imagerysection == 0);
        d_imagerysection = new ImagerySection(attributes.getValueAsString(NameAttribute));

        CEGUI_LOGINSANE("-----> Start of definition for imagery section '" + d_imagerysection->getName() + "'.");
    }

    /*************************************************************************
        Method that handles the opening StateImagery XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementStateImageryStart(const XMLAttributes& attributes)
    {
        assert(d_stateimagery == 0);
        d_stateimagery = new StateImagery(attributes.getValueAsString(NameAttribute));
        d_stateimagery->setClippedToDisplay(!attributes.getValueAsBool(ClippedAttribute, true));

        CEGUI_LOGINSANE("-----> Start of definition for imagery for state '" + d_stateimagery->getName() + "'.");
    }

    /*************************************************************************
        Method that handles the opening Layer XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementLayerStart(const XMLAttributes& attributes)
    {
        assert(d_layer == 0);
        d_layer = new LayerSpecification(attributes.getValueAsInteger(PriorityAttribute, 0));

        CEGUI_LOGINSANE("-------> Start of definition of new imagery layer, priority: " + attributes.getValueAsString(PriorityAttribute, "0"));
    }

    /*************************************************************************
        Method that handles the opening Section XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementSectionStart(const XMLAttributes& attributes)
    {
        assert(d_section == 0);
        assert(d_widgetlook != 0);
        String owner(attributes.getValueAsString(LookAttribute));
        d_section =
            new SectionSpecification(owner.empty() ? d_widgetlook->getName() : owner,
                                     attributes.getValueAsString(SectionNameAttribute),
                                     attributes.getValueAsString(ControlPropertyAttribute),
                                     attributes.getValueAsString(ControlValueAttribute),
                                     attributes.getValueAsString(ControlWidgetAttribute));

        CEGUI_LOGINSANE("---------> Layer references imagery section '" + d_section->getSectionName() + "'.");
    }

    /*************************************************************************
        Method that handles the opening ImageryComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageryComponentStart(const XMLAttributes&)
    {
        assert(d_imagerycomponent == 0);
        d_imagerycomponent = new ImageryComponent();

        CEGUI_LOGINSANE("-------> Image component definition...");
    }

    /*************************************************************************
        Method that handles the opening TextComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextComponentStart(const XMLAttributes&)
    {
        assert(d_textcomponent == 0);
        d_textcomponent = new TextComponent();

        CEGUI_LOGINSANE("-------> Text component definition...");
    }

    /*************************************************************************
        Method that handles the opening FrameComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFrameComponentStart(const XMLAttributes&)
    {
        assert(d_framecomponent == 0);
        d_framecomponent = new FrameComponent();

        CEGUI_LOGINSANE("-------> Frame component definition...");
    }

    /*************************************************************************
        Method that handles the opening Area XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAreaStart(const XMLAttributes&)
    {
        assert(d_area == 0);
        d_area = new ComponentArea();
    }

    /*************************************************************************
        Method that handles the opening Image XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageStart(const XMLAttributes& attributes)
    {
        if (d_imagerycomponent)
        {
            d_imagerycomponent->setImage(attributes.getValueAsString(NameAttribute));
            CEGUI_LOGINSANE("---------> Using image: " + attributes.getValueAsString(NameAttribute));
        }
        else if (d_framecomponent)
        {
            d_framecomponent->setImage(
                FalagardXMLHelper<FrameImageComponent>::fromString(
                    attributes.getValueAsString(ComponentAttribute)),
                attributes.getValueAsString(NameAttribute));

            CEGUI_LOGINSANE("---------> Using image: " +
                attributes.getValueAsString(NameAttribute) + " for: " +
                attributes.getValueAsString(ComponentAttribute));
        }
    }

    /*************************************************************************
        Method that handles the opening Colours XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColoursStart(const XMLAttributes& attributes)
    {
        ColourRect cols(
            hexStringToARGB(attributes.getValueAsString(TopLeftAttribute)),
            hexStringToARGB(attributes.getValueAsString(TopRightAttribute)),
            hexStringToARGB(attributes.getValueAsString(BottomLeftAttribute)),
            hexStringToARGB(attributes.getValueAsString(BottomRightAttribute)));

        assignColours(cols);
    }

    /*************************************************************************
        Method that handles the opening VertFormat XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementVertFormatStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            const FrameImageComponent what =
                FalagardXMLHelper<FrameImageComponent>::fromString(
                    attributes.getValueAsString(ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::Background));
            const VerticalFormatting fmt =
                FalagardXMLHelper<VerticalFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute));

            switch(what)
            {
                case FIC_LEFT_EDGE:
                    d_framecomponent->setLeftEdgeFormatting(fmt);
                    break;
                case FIC_RIGHT_EDGE:
                    d_framecomponent->setRightEdgeFormatting(fmt);
                    break;
                case FIC_BACKGROUND:
                    d_framecomponent->setBackgroundVerticalFormatting(fmt);
                    break;
                default:
                    CEGUI_THROW(InvalidRequestException(
                        VertFormatElement + " within " +
                        FrameComponentElement + " may only be used for "
                        "LeftEdge, RightEdge or Background components. "
                        "Received: " +
                        attributes.getValueAsString(ComponentAttribute)));
            }
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setVerticalFormatting(
                FalagardXMLHelper<VerticalFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setVerticalFormatting(
                FalagardXMLHelper<VerticalTextFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute)));
        }
    }

    /*************************************************************************
        Method that handles the opening HorzFormat XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementHorzFormatStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            const FrameImageComponent what =
                FalagardXMLHelper<FrameImageComponent>::fromString(
                    attributes.getValueAsString(ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::Background));
            const HorizontalFormatting fmt =
                FalagardXMLHelper<HorizontalFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute));

            switch(what)
            {
                case FIC_TOP_EDGE:
                    d_framecomponent->setTopEdgeFormatting(fmt);
                    break;
                case FIC_BOTTOM_EDGE:
                    d_framecomponent->setBottomEdgeFormatting(fmt);
                    break;
                case FIC_BACKGROUND:
                    d_framecomponent->setBackgroundHorizontalFormatting(fmt);
                    break;
                default:
                    CEGUI_THROW(InvalidRequestException(
                        HorzFormatElement + " within " +
                        FrameComponentElement + " may only be used for "
                        "TopEdge, BottomEdge or Background components. "
                        "Received: " +
                        attributes.getValueAsString(ComponentAttribute)));
            }
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setHorizontalFormatting(
                FalagardXMLHelper<HorizontalFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setHorizontalFormatting(
                FalagardXMLHelper<HorizontalTextFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute)));
        }
    }

    /*************************************************************************
        Method that handles the opening VertAlignment XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementVertAlignmentStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent != 0);
        d_childcomponent->setVerticalWidgetAlignment(
            FalagardXMLHelper<VerticalAlignment>::fromString(
                attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening HorzAlignment XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementHorzAlignmentStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent != 0);
        d_childcomponent->setHorizontalWidgetAlignment(
            FalagardXMLHelper<HorizontalAlignment>::fromString(
                attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening Property XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook != 0);
        PropertyInitialiser prop(attributes.getValueAsString(NameAttribute), attributes.getValueAsString(ValueAttribute));

        if (d_childcomponent)
        {
            d_childcomponent->addPropertyInitialiser(prop);
            CEGUI_LOGINSANE("-------> Added property initialiser for property: " + prop.getTargetPropertyName() + " with value: " + prop.getInitialiserValue());
        }
        else
        {
            d_widgetlook->addPropertyInitialiser(prop);
            CEGUI_LOGINSANE("---> Added property initialiser for property: " + prop.getTargetPropertyName() + " with value: " + prop.getInitialiserValue());
        }
    }

    /*************************************************************************
        Method that handles the opening Dim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementDimStart(const XMLAttributes& attributes)
    {
        d_dimension.setDimensionType(
            FalagardXMLHelper<DimensionType>::fromString(
                attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening UnifiedDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementUnifiedDimStart(const XMLAttributes& attributes)
    {
        UnifiedDim base(
            UDim(attributes.getValueAsFloat(ScaleAttribute, 0.0f),
                 attributes.getValueAsFloat(OffsetAttribute, 0.0f)),
            FalagardXMLHelper<DimensionType>::fromString(
                attributes.getValueAsString(TypeAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening AbsoluteDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAbsoluteDimStart(const XMLAttributes& attributes)
    {
        AbsoluteDim base(attributes.getValueAsFloat(ValueAttribute, 0.0f));
        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening ImageDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageDimStart(const XMLAttributes& attributes)
    {
        ImageDim base(attributes.getValueAsString(NameAttribute),
                      FalagardXMLHelper<DimensionType>::fromString(
                          attributes.getValueAsString(DimensionAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening ImageDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagePropertyDimStart(const XMLAttributes& attributes)
    {
        ImagePropertyDim base(
            attributes.getValueAsString(NameAttribute),
            FalagardXMLHelper<DimensionType>::fromString(
                attributes.getValueAsString(DimensionAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening WidgetDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementWidgetDimStart(const XMLAttributes& attributes)
    {
        WidgetDim base(attributes.getValueAsString(WidgetAttribute),
                       FalagardXMLHelper<DimensionType>::fromString(
                           attributes.getValueAsString(DimensionAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening FontDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFontDimStart(const XMLAttributes& attributes)
    {
        FontDim base(
            attributes.getValueAsString(WidgetAttribute),
            attributes.getValueAsString(FontAttribute),
            attributes.getValueAsString(StringAttribute),
            FalagardXMLHelper<FontMetricType>::fromString(
                attributes.getValueAsString(TypeAttribute)),
            attributes.getValueAsFloat(PaddingAttribute, 0.0f));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening PropertyDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyDimStart(const XMLAttributes& attributes)
    {
        String str_type = attributes.getValueAsString(TypeAttribute);
        DimensionType type = DT_INVALID;
        if (!str_type.empty())
            type = FalagardXMLHelper<DimensionType>::fromString(str_type);

        PropertyDim base(attributes.getValueAsString(WidgetAttribute),
                         attributes.getValueAsString(NameAttribute),
                         type);

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening Text XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextStart(const XMLAttributes& attributes)
    {
        assert (d_textcomponent != 0);
        d_textcomponent->setText(attributes.getValueAsString(StringAttribute));
        d_textcomponent->setFont(attributes.getValueAsString(FontAttribute));
    }

    /*************************************************************************
        Method that handles the opening ColourRectProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColourRectPropertyStart(const XMLAttributes& attributes)
    {
        // need to decide what to apply colours to
        if (d_framecomponent)
        {
            d_framecomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
        }
        else if (d_imagerysection)
        {
            d_imagerysection->setMasterColoursPropertySource(attributes.getValueAsString(NameAttribute));
        }
        else if (d_section)
        {
            d_section->setOverrideColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_section->setUsingOverrideColours(true);
        }
    }

    /*************************************************************************
        Method that handles the opening NamedArea XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementNamedAreaStart(const XMLAttributes& attributes)
    {
        assert(d_namedArea == 0);
        d_namedArea = new NamedArea(attributes.getValueAsString(NameAttribute));

        CEGUI_LOGINSANE("-----> Creating named area: " + d_namedArea->getName());
    }

    /*************************************************************************
        Method that handles the opening PropertyDefinition XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyDefinitionStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook);
        PropertyDefinitionBase* prop;

        const String name(attributes.getValueAsString(NameAttribute));
        const String init(attributes.getValueAsString(InitialValueAttribute));
        const String help(attributes.getValueAsString(HelpStringAttribute,
                                                      PropertyDefinitionHelpDefaultValue));
        const String type(attributes.getValueAsString(TypeAttribute, GenericDataType));
        bool redraw(attributes.getValueAsBool(RedrawOnWriteAttribute, false));
        bool layout(attributes.getValueAsBool(LayoutOnWriteAttribute, false));
        const String eventName(attributes.getValueAsString(FireEventAttribute));
        typedef std::pair<float, float> Range;

        if(type == PropertyHelper<Colour>::getDataTypeName())
            prop = new PropertyDefinition<Colour>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName() );
        else if(type == PropertyHelper<ColourRect>::getDataTypeName())
            prop = new PropertyDefinition<ColourRect>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<UBox>::getDataTypeName())
            prop = new PropertyDefinition<UBox>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<URect>::getDataTypeName())
            prop = new PropertyDefinition<URect>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<USize>::getDataTypeName())
            prop = new PropertyDefinition<USize>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<UDim>::getDataTypeName())
            prop = new PropertyDefinition<UDim>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<UVector2>::getDataTypeName())
            prop = new PropertyDefinition<UVector2>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<Sizef>::getDataTypeName())
            prop = new PropertyDefinition<Sizef>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<glm::vec2>::getDataTypeName())
            prop = new PropertyDefinition<glm::vec2>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<glm::vec3>::getDataTypeName())
            prop = new PropertyDefinition<glm::vec3>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<Rectf>::getDataTypeName())
            prop = new PropertyDefinition<Rectf>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<Font*>::getDataTypeName())
            prop = new PropertyDefinition<Font*>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<Image*>::getDataTypeName())
            prop = new PropertyDefinition<Image*>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<glm::quat>::getDataTypeName())
            prop = new PropertyDefinition<glm::quat>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<AspectMode>::getDataTypeName())
            prop = new PropertyDefinition<AspectMode>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<HorizontalAlignment>::getDataTypeName())
            prop = new PropertyDefinition<HorizontalAlignment>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<VerticalAlignment>::getDataTypeName())
            prop = new PropertyDefinition<VerticalAlignment>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<HorizontalTextFormatting>::getDataTypeName())
            prop = new PropertyDefinition<HorizontalTextFormatting>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<VerticalTextFormatting>::getDataTypeName())
            prop = new PropertyDefinition<VerticalTextFormatting>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<WindowUpdateMode>::getDataTypeName())
            prop = new PropertyDefinition<WindowUpdateMode>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<bool>::getDataTypeName())
            prop = new PropertyDefinition<bool>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<uint>::getDataTypeName())
            prop = new PropertyDefinition<uint>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<unsigned long>::getDataTypeName())
            prop = new PropertyDefinition<unsigned long>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<int>::getDataTypeName())
            prop = new PropertyDefinition<int>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<float>::getDataTypeName())
            prop = new PropertyDefinition<float>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<double>::getDataTypeName())
            prop = new PropertyDefinition<double>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<TabControl::TabPanePosition>::getDataTypeName())
            prop = new PropertyDefinition<TabControl::TabPanePosition>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<Spinner::TextInputMode>::getDataTypeName())
            prop = new PropertyDefinition<Spinner::TextInputMode>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<ItemListBase::SortMode>::getDataTypeName())
            prop = new PropertyDefinition<ItemListBase::SortMode>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<ListHeaderSegment::SortDirection>::getDataTypeName())
            prop = new PropertyDefinition<ListHeaderSegment::SortDirection>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<MultiColumnList::SelectionMode>::getDataTypeName())
            prop = new PropertyDefinition<MultiColumnList::SelectionMode>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<VerticalFormatting>::getDataTypeName())
            prop = new PropertyDefinition<VerticalFormatting>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<HorizontalFormatting>::getDataTypeName())
            prop = new PropertyDefinition<HorizontalFormatting>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if(type == PropertyHelper<Range>::getDataTypeName())
            prop = new PropertyDefinition<Range>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else
        {
            if (type != GenericDataType && type != "String")
            {
                // type was specified but wasn't recognised
                Logger::getSingleton().logEvent("Type '" + type + "' wasn't recognized in property definition (name: '" + name + "').", Warnings);
            }

            prop = new PropertyDefinition<String>(name, init, help, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        }

        CEGUI_LOGINSANE("-----> Adding PropertyDefiniton. Name: " + name + " Default Value: " + init);

        d_widgetlook->addPropertyDefinition(prop);
    }

    /*************************************************************************
        Method that handles the opening PropertyLinkDefinition XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyLinkDefinitionStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook);
        assert(d_propertyLink == 0);

        const String widget(attributes.getValueAsString(WidgetAttribute));
        const String target(attributes.getValueAsString(TargetPropertyAttribute));
        const String name(attributes.getValueAsString(NameAttribute));
        const String init(attributes.getValueAsString(InitialValueAttribute));
        const String type(attributes.getValueAsString(TypeAttribute, GenericDataType));
        bool redraw(attributes.getValueAsBool(RedrawOnWriteAttribute, false));
        bool layout(attributes.getValueAsBool(LayoutOnWriteAttribute, false));
        const String eventName(attributes.getValueAsString(FireEventAttribute));
        typedef std::pair<float, float> Range;

        if (type == PropertyHelper<Colour>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<Colour>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<ColourRect>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<ColourRect>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<UBox>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<UBox>(name, widget,
                    target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<URect>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<URect>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<USize>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<USize>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<UDim>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<UDim>(name, widget,
                    target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<UVector2>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<UVector2>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<Sizef>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<Sizef>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<glm::vec2>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<glm::vec2>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<glm::vec3>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<glm::vec3>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<Rectf>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<Rectf>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<Font*>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<Font*>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<Image*>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<Image*>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<glm::quat>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<glm::quat>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<AspectMode>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<AspectMode>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<HorizontalAlignment>::getDataTypeName())
            d_propertyLink =new PropertyLinkDefinition<HorizontalAlignment>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<VerticalAlignment>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<VerticalAlignment>(
                    name, widget, target, init, d_widgetlook->getName(), redraw,
                    layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<HorizontalTextFormatting>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    HorizontalTextFormatting>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<VerticalTextFormatting>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    VerticalTextFormatting>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<WindowUpdateMode>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<WindowUpdateMode>(
                    name, widget, target, init, d_widgetlook->getName(), redraw,
                    layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<bool>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<bool>(name, widget,
                    target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<uint>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<uint>(name, widget,
                    target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<unsigned long>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<unsigned long>(
                    name, widget, target, init, d_widgetlook->getName(), redraw,
                    layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<int>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<int>(name, widget,
                    target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<float>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<float>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<double>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<double>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<TabControl::TabPanePosition>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    TabControl::TabPanePosition>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<Spinner::TextInputMode>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    Spinner::TextInputMode>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<ItemListBase::SortMode>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    ItemListBase::SortMode>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<ListHeaderSegment::SortDirection>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    ListHeaderSegment::SortDirection>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<MultiColumnList::SelectionMode>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<
                    MultiColumnList::SelectionMode>(name, widget, target, init,
                    d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else if (type == PropertyHelper<VerticalFormatting>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<VerticalFormatting>(
                    name, widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName()
            );
        else if (type == PropertyHelper<HorizontalFormatting>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<HorizontalFormatting>(
                    name, widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName()
            );
        else if (type == PropertyHelper<Range>::getDataTypeName())
            d_propertyLink = new PropertyLinkDefinition<Range>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        else
        {
            if (type != GenericDataType && type != PropertyHelper<String>::getDataTypeName())
            {
                // type was specified but wasn't recognised
                Logger::getSingleton().logEvent("Type '" + type + "' wasn't recognized in property link definition (name: '" + name + "').", Warnings);
            }

            d_propertyLink = new PropertyLinkDefinition<String>(name,
                    widget, target, init, d_widgetlook->getName(), redraw, layout, eventName, d_widgetlook->getName());
        }
        CEGUI_LOGINSANE("-----> Adding PropertyLinkDefiniton. Name: " +
                        name);

        if (!widget.empty() || !target.empty())
        {
            CEGUI_LOGINSANE("-------> Adding link target to property: " + target +
                        " on widget: " + widget);
        }
    }

    /*************************************************************************
        Method that handles the opening OperatorDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementOperatorDimStart(const XMLAttributes& attributes)
    {
        OperatorDim base(FalagardXMLHelper<DimensionOperator>::fromString(
            attributes.getValueAsString(OperatorAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening VertFormatProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementVertFormatPropertyStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            const FrameImageComponent what =
                FalagardXMLHelper<FrameImageComponent>::fromString(
                    attributes.getValueAsString(ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::Background));
            const VerticalFormatting fmt =
                FalagardXMLHelper<VerticalFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute));

            switch(what)
            {
                case FIC_LEFT_EDGE:
                    d_framecomponent->setLeftEdgeFormatting(fmt);
                    break;
                case FIC_RIGHT_EDGE:
                    d_framecomponent->setRightEdgeFormatting(fmt);
                    break;
                case FIC_BACKGROUND:
                    d_framecomponent->setBackgroundVerticalFormatting(fmt);
                    break;
                default:
                    CEGUI_THROW(InvalidRequestException(
                        VertFormatPropertyElement + " within " +
                        FrameComponentElement + " may only be used for "
                        "LeftEdge, RightEdge or Background components. "
                        "Received: " +
                        attributes.getValueAsString(ComponentAttribute)));
            }
        }
        else if (d_imagerycomponent)
            d_imagerycomponent->setVerticalFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_textcomponent)
            d_textcomponent->setVerticalFormattingPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening HorzFormatProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementHorzFormatPropertyStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            const FrameImageComponent what =
                FalagardXMLHelper<FrameImageComponent>::fromString(
                    attributes.getValueAsString(ComponentAttribute, FalagardXMLHelper<FrameImageComponent>::Background));
            const HorizontalFormatting fmt =
                FalagardXMLHelper<HorizontalFormatting>::fromString(
                    attributes.getValueAsString(TypeAttribute));

            switch(what)
            {
                case FIC_TOP_EDGE:
                    d_framecomponent->setTopEdgeFormatting(fmt);
                    break;
                case FIC_BOTTOM_EDGE:
                    d_framecomponent->setBottomEdgeFormatting(fmt);
                    break;
                case FIC_BACKGROUND:
                    d_framecomponent->setBackgroundHorizontalFormatting(fmt);
                    break;
                default:
                    CEGUI_THROW(InvalidRequestException(
                        HorzFormatPropertyElement + " within " +
                        FrameComponentElement + " may only be used for "
                        "TopEdge, BottomEdge or Background components. "
                        "Received: " +
                        attributes.getValueAsString(ComponentAttribute)));
            }
        }
        else if (d_imagerycomponent)
            d_imagerycomponent->setHorizontalFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_textcomponent)
            d_textcomponent->setHorizontalFormattingPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening AreaProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAreaPropertyStart(const XMLAttributes& attributes)
    {
        assert (d_area != 0);

        d_area->setAreaPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening ImageProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagePropertyStart(const XMLAttributes& attributes)
    {
        assert(d_imagerycomponent != 0 || d_framecomponent != 0);

        if (d_imagerycomponent)
        {
            d_imagerycomponent->setImagePropertySource(
                attributes.getValueAsString(NameAttribute));

            CEGUI_LOGINSANE("---------> Using image via property: " +
                attributes.getValueAsString(NameAttribute));
        }
        else if (d_framecomponent)
        {
            d_framecomponent->setImagePropertySource(
                FalagardXMLHelper<FrameImageComponent>::fromString(
                    attributes.getValueAsString(ComponentAttribute)),
                attributes.getValueAsString(NameAttribute));

            CEGUI_LOGINSANE("---------> Using image via property: " +
                attributes.getValueAsString(NameAttribute) + " for: " +
                attributes.getValueAsString(ComponentAttribute));
        }
    }

    /*************************************************************************
        Method that handles the opening TextProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_textcomponent != 0);

        d_textcomponent->setTextPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening FontProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFontPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_textcomponent != 0);

        d_textcomponent->setFontPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening Colours XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColourStart(const XMLAttributes& attributes)
    {
        ColourRect cols(hexStringToARGB(attributes.getValueAsString(ColourAttribute)));
        assignColours(cols);
    }

    /*************************************************************************
        Method that handles the closing Falagard XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFalagardEnd()
    {
        Logger::getSingleton().logEvent("===== Look and feel parsing completed =====");
    }

    /*************************************************************************
        Method that handles the closing WidgetLook XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementWidgetLookEnd()
    {
        if (d_widgetlook)
        {
            Logger::getSingleton().logEvent("---< End of definition for widget look '" + d_widgetlook->getName() + "'.", Informative);
            d_manager->addWidgetLook(*d_widgetlook);
            delete d_widgetlook;
            d_widgetlook = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Child XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementChildEnd()
    {
        assert(d_widgetlook != 0);

        if (d_childcomponent)
        {
            CEGUI_LOGINSANE("-----< End of definition for child widget. Type: " + d_childcomponent->getTargetType() + ".");
            d_widgetlook->addWidgetComponent(*d_childcomponent);
            delete d_childcomponent;
            d_childcomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing ImagerySection XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagerySectionEnd()
    {
        assert(d_widgetlook != 0);

        if (d_imagerysection)
        {
            CEGUI_LOGINSANE("-----< End of definition for imagery section '" + d_imagerysection->getName() + "'.");
            d_widgetlook->addImagerySection(*d_imagerysection);
            delete d_imagerysection;
            d_imagerysection = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing StateImagery XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementStateImageryEnd()
    {
        assert(d_widgetlook != 0);

        if (d_stateimagery)
        {
            CEGUI_LOGINSANE("-----< End of definition for imagery for state '" + d_stateimagery->getName() + "'.");
            d_widgetlook->addStateImagery(*d_stateimagery);
            delete d_stateimagery;
            d_stateimagery = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Layer XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementLayerEnd()
    {
        assert(d_stateimagery != 0);

        if (d_layer)
        {
            CEGUI_LOGINSANE("-------< End of definition of imagery layer.");
            d_stateimagery->addLayer(*d_layer);
            delete d_layer;
            d_layer = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Section XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementSectionEnd()
    {
        assert(d_layer != 0);

        if (d_section)
        {
            d_layer->addSectionSpecification(*d_section);
            delete d_section;
            d_section = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing ImageryComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageryComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_imagerycomponent)
        {
            d_imagerysection->addImageryComponent(*d_imagerycomponent);
            delete d_imagerycomponent;
            d_imagerycomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing TextComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_textcomponent)
        {
            d_imagerysection->addTextComponent(*d_textcomponent);
            delete d_textcomponent;
            d_textcomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing FrameComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFrameComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_framecomponent)
        {
            d_imagerysection->addFrameComponent(*d_framecomponent);
            delete d_framecomponent;
            d_framecomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Area XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAreaEnd()
    {
        assert((d_childcomponent != 0) || (d_imagerycomponent != 0) || (d_textcomponent != 0) || d_namedArea != 0 || d_framecomponent != 0);
        assert(d_area != 0);

        if (d_childcomponent)
        {
            d_childcomponent->setComponentArea(*d_area);
        }
        else if (d_framecomponent)
        {
            d_framecomponent->setComponentArea(*d_area);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setComponentArea(*d_area);
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setComponentArea(*d_area);
        }
        else if (d_namedArea)
        {
            d_namedArea->setArea(*d_area);
        }

        delete d_area;
        d_area = 0;
    }

    /*************************************************************************
        Method that handles the closing NamedArea XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementNamedAreaEnd()
    {
        assert(d_widgetlook != 0);

        if (d_namedArea)
        {
            d_widgetlook->addNamedArea(*d_namedArea);
            delete d_namedArea;
            d_namedArea = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing of all *Dim XML elements.
    *************************************************************************/
    void Falagard_xmlHandler::elementAnyDimEnd()
    {
        if (!d_dimStack.empty())
        {
            BaseDim* currDim = d_dimStack.back();
            d_dimStack.pop_back();

            if (!d_dimStack.empty())
            {
                if (OperatorDim* op = dynamic_cast<OperatorDim*>(d_dimStack.back()))
                   op->setNextOperand(currDim);
            }
            else
            {
                d_dimension.setBaseDimension(*currDim);
                assignAreaDimension(d_dimension);
            }

            // release the dim we popped.
            delete currDim;
        }
    }

    void Falagard_xmlHandler::elementPropertyLinkDefinitionEnd()
    {
        assert(d_propertyLink);
        d_widgetlook->addPropertyLinkDefinition(d_propertyLink);

        CEGUI_LOGINSANE("<----- End of PropertyLinkDefiniton. Name: " +
                        d_propertyLink->getPropertyName());
        d_propertyLink = 0;
    }

    void Falagard_xmlHandler::elementPropertyLinkTargetStart(const XMLAttributes& attributes)
    {
        assert(d_propertyLink);

        const String w(attributes.getValueAsString(WidgetAttribute));
        const String p(attributes.getValueAsString(PropertyAttribute));

        if (!w.empty() || !p.empty())
        {
            const String type(dynamic_cast<Property*>(d_propertyLink)->getDataType());

            typedef std::pair<float,float> Range;

            if(type == PropertyHelper<Colour>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Colour>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<ColourRect>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<ColourRect>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<UBox>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<UBox>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<URect>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<URect>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<USize>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<USize>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<UDim>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<UDim>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<UVector2>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<UVector2>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<Sizef>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Sizef>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<glm::vec2>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<glm::vec2>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<glm::vec3>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<glm::vec3>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<Rectf>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Rectf>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<Font*>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Font*>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<Image*>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Image*>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<glm::quat>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<glm::quat>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<AspectMode>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<AspectMode>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<HorizontalAlignment>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<HorizontalAlignment>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<VerticalAlignment>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<VerticalAlignment>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<HorizontalTextFormatting>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<HorizontalTextFormatting>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<VerticalTextFormatting>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<VerticalTextFormatting>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<WindowUpdateMode>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<WindowUpdateMode>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<bool>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<bool>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<uint>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<uint>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<unsigned long>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<unsigned long>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<uint>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<uint>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<int>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<int>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<float>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<float>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<double>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<double>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<TabControl::TabPanePosition>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<TabControl::TabPanePosition>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<Spinner::TextInputMode>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Spinner::TextInputMode>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<ItemListBase::SortMode>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<ItemListBase::SortMode>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<ListHeaderSegment::SortDirection>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<ListHeaderSegment::SortDirection>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<MultiColumnList::SelectionMode>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<MultiColumnList::SelectionMode>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<VerticalFormatting>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<VerticalFormatting>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<HorizontalFormatting>::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<HorizontalFormatting>* >(d_propertyLink)->addLinkTarget(w, p);
            else if(type == PropertyHelper<std::pair<float,float> >::getDataTypeName())
                dynamic_cast<PropertyLinkDefinition<Range>* >(d_propertyLink)->addLinkTarget(w, p);
            else
                dynamic_cast<PropertyLinkDefinition<String>* >(d_propertyLink)->addLinkTarget(w, p);

            CEGUI_LOGINSANE("-------> Adding link target to property: " + p +
                        " on widget: " + w);
        }
    }

    void Falagard_xmlHandler::elementAnimationDefinitionStart(
                                            const XMLAttributes& attributes)
    {
        assert(d_widgetlook != 0);

        String anim_name_prefix(d_widgetlook->getName());
        anim_name_prefix.append("/");

        const String anim_name(anim_name_prefix +
                        attributes.getValueAsString(NameAttribute));


        if (AnimationManager::getSingleton().isAnimationPresent(anim_name))
        {
            Logger::getSingleton().logEvent(
                "[XMLHandler] WARNING: Using existing Animation :" + anim_name);
        }
        else
        {
            d_chainedHandler = new AnimationDefinitionHandler(
                attributes, anim_name_prefix);
        }

        // This is a little bit of abuse here, ideally we would get the name
        // somewhere else.
        d_widgetlook->addAnimationName(
            anim_name_prefix +
            attributes.getValueAsString("name"));
    }


    void Falagard_xmlHandler::elementEventLinkDefinitionStart(
                                                const XMLAttributes& attributes)
    {
        assert(d_widgetlook);
        assert(d_eventLink == 0);

        const String widget(attributes.getValueAsString(WidgetAttribute));
        const String event(attributes.getValueAsString(EventAttribute));

        d_eventLink = new EventLinkDefinition(
            attributes.getValueAsString(NameAttribute));

        CEGUI_LOGINSANE("-----> Adding EventLinkDefiniton. Name: " +
                        d_eventLink->getName());

        processEventLinkTarget(widget, event);
    }

    void Falagard_xmlHandler::processEventLinkTarget(const String& widget, const String& event)
    {
        assert(d_eventLink);

        if (!widget.empty() || !event.empty())
        {
            d_eventLink->addLinkTarget(widget, event);
            CEGUI_LOGINSANE("-------> Adding link target to event: " + event +
                        " on widget: " + widget);
        }
    }

    void Falagard_xmlHandler::elementEventLinkTargetStart(
                                                const XMLAttributes& attributes)
    {
        const String widget(attributes.getValueAsString(WidgetAttribute));
        const String event(attributes.getValueAsString(EventAttribute));

        processEventLinkTarget(widget, event);
    }

    void Falagard_xmlHandler::elementEventLinkDefinitionEnd()
    {
        assert(d_eventLink);
        d_widgetlook->addEventLinkDefinition(*d_eventLink);

        CEGUI_LOGINSANE("<----- End of EventLinkDefiniton. Name: " +
                        d_eventLink->getName());

        delete d_eventLink;
        d_eventLink = 0;
    }

    void Falagard_xmlHandler::elementNamedAreaSourceStart(const XMLAttributes& attributes)
    {
        assert (d_area != 0);

        const String look(attributes.getValueAsString(LookAttribute));

        d_area->setNamedAreaSource(look.empty() ? d_widgetlook->getName() : look,
                                  attributes.getValueAsString(NameAttribute));
    }

    void Falagard_xmlHandler::elementEventActionStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent != 0);

        const EventAction action(
            attributes.getValueAsString(EventAttribute),
            FalagardXMLHelper<ChildEventAction>::fromString(
                attributes.getValueAsString(ActionAttribute)));

        d_childcomponent->addEventAction(action);
    }

    /*************************************************************************
        register a handler for the opening tag of an XML element
    *************************************************************************/
    void Falagard_xmlHandler::registerElementStartHandler(const String& element, ElementStartHandler handler)
    {
        d_startHandlersMap[element] = handler;
    }

    /*************************************************************************
        register a handler for the closing tag of an XML element
    *************************************************************************/
    void Falagard_xmlHandler::registerElementEndHandler(const String& element, ElementEndHandler handler)
    {
        d_endHandlersMap[element] = handler;
    }

} // End of  CEGUI namespace section
