/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

//#include "nui.h"
#include "nglString.h"
#include <map>
#include <list>
#include "nuiContainer.h"

class nuiBuilder;

typedef nuiWidgetPtr (*nuiCreateWidgetFn)();

class nuiWidgetDesc
{
public:
  nuiWidgetDesc(){};
  nuiWidgetDesc(const nglString& rClassName, const nglString& rClassGroup, nuiCreateWidgetFn pHandler)
  {
    mClassName = rClassName;
    mClassGroup = rClassGroup;
    mpHandler = pHandler;
  };
  
  ~nuiWidgetDesc(){};

  const nglString& GetClassName() const
  {
    return mClassName;
  };
  
  const nglString& GetClassGroup() const
  {
    return mClassGroup;
  };
  
  nuiCreateWidgetFn GetHandler() const
  {
    return mpHandler;
  };
  
  
  
private : 

  nglString mClassName;
  nglString mClassGroup;
  nuiCreateWidgetFn mpHandler;
};


typedef std::map<nglString, nuiWidgetDesc, nglString::LessFunctor> nuiBuilderMap;




///////////////////////////////////////////////////////////////////

class NUI_API nuiBuilder
{
public:
  void SetHandler(const nglString& ClassName, const nglString& ClassGroup, nuiCreateWidgetFn pHandler); ///< This method permits to add or override a widget creation function.
  nuiCreateWidgetFn GetHandler(const nglString& ClassName) const; ///< This method retrieves thewidget creation function associated with a class name.
  bool GetClassList(std::list<nuiWidgetDesc>& rClassNames) const; ///< This method fills the given nuiWidgetDesc list with the description (name and group) of the classes that this map can handle. 

  nuiWidget* CreateWidget(const nglString& rClassName) const;
  
  static nuiBuilder& Get()
  {
    return mBuilder;
  }
protected:
  nuiBuilderMap mBuilderMap;
  static nuiBuilder mBuilder;


private:
  nuiBuilder();
  ~nuiBuilder();
  
  void Init();
};

template <class T>
nuiWidgetPtr nuiWidgetCreateFunction()
{
  return new T();
}

#define NUI_ADD_WIDGET_CREATOR(X,Y) { nuiBuilder::Get().SetHandler(_T(#X),_T(Y),&nuiWidgetCreateFunction<X>); }

nuiWidget* nuiCreateWidget(const nglString& rClassName);
nuiWidget* nuiCreateWidget(const nuiXMLNode* pNode);

///////////////////////////////////////////////////////////////////
class nuiWidgetCreatorOperation;

class nuiWidgetCreator
{
public:
  nuiWidgetCreator(const nglString& rClassName, const nglString& rObjectName);
  virtual ~nuiWidgetCreator();
  
  nuiWidget* Create(const std::map<nglString, nglString>& rParamDictionnary, nuiBuilder* pBuilder = NULL) const;
  nuiWidget* Create(nuiBuilder* pBuilder = NULL) const;
  
  // For any simple container widget:
  void AddChild(nuiWidgetCreator* pCreator);
  
  // For box containers (nuiBox: nuiHBox, nuiVBox):
  void AddCell(nuiWidgetCreator* pCreator);
  void SetCell(uint32 cell, nuiWidgetCreator* pCreator);

  // For nuiGrid based containers
  void SetCell(uint32 column, uint32 row, nuiWidgetCreator* pCreator);
  
  void SetProperty(const nglString& rName, const nglString& rValue);
  void SetAttribute(const nglString& rName, const nglString& rValue);
protected:
  std::vector<nuiWidgetCreatorOperation> mOperations;
  nglString mClassName;
  nglString mObjectName;
};





