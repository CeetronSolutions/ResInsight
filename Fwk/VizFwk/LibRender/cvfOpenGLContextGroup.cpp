//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2013 Ceetron AS
//
//   This library may be used under the terms of either the GNU General Public License or
//   the GNU Lesser General Public License as follows:
//
//   GNU General Public License Usage
//   This library is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>>
//   for more details.
//
//   GNU Lesser General Public License Usage
//   This library is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation; either version 2.1 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU Lesser General Public License at <<http://www.gnu.org/licenses/lgpl-2.1.html>>
//   for more details.
//
//##################################################################################################


#include "cvfBase.h"
#include "cvfOpenGLContextGroup.h"
#include "cvfOpenGL.h"
#include "cvfOpenGLContext.h"
#include "cvfOpenGLResourceManager.h"
#include "cvfOpenGLCapabilities.h"
#include "cvfLogManager.h"
#include "cvfTrace.h"

#include <memory.h>

#ifdef WIN32
#pragma warning (push)
#pragma warning (disable: 4668)
#include "glew/GL/wglew.h"
#pragma warning (pop)
#endif


namespace cvf {


//==================================================================================================
///
/// \class cvf::OpenGLContextGroup
/// \ingroup Render
///
/// A context group associates OpenGLContext instances that share OpenGL resources such as
/// shader objects, textures and buffer objects. Contexts added to the group must be compatible from
/// OpenGL's perspective - that is they must use identical (pixel) formats.
/// 
//==================================================================================================

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
OpenGLContextGroup::OpenGLContextGroup()
:   m_isInitialized(false),
    m_glewContextStruct(NULL),
    m_wglewContextStruct(NULL)
{
    //Trace::show("OpenGLContextGroup constructor");

    m_resourceManager = new OpenGLResourceManager;
    m_logger = CVF_GET_LOGGER("cee.cvf.OpenGL"); 
    m_capabilities = new OpenGLCapabilities;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
OpenGLContextGroup::~OpenGLContextGroup()
{
    //Trace::show("OpenGLContextGroup destructor");

    // Our resource manager must already be clean
    // There is no way of safely deleting the resources after all the contexts have been removed
    CVF_ASSERT(m_resourceManager.notNull());
    CVF_ASSERT(!m_resourceManager->hasAnyOpenGLResources());

    // We're about to die, so make sure we disentangle from the contexts
    // This is important since all the contexts have back raw pointers to the group
    size_t numContexts = m_contexts.size();
    size_t i;
    for (i = 0; i < numContexts; i++)
    {
        OpenGLContext* ctx = m_contexts.at(i);
        ctx->m_contextGroup = NULL;
    }

    m_contexts.clear();

    if (m_isInitialized)
    {
        uninitializeContextGroup();
    }
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool OpenGLContextGroup::isContextGroupInitialized() const
{
    return m_isInitialized;
}


//--------------------------------------------------------------------------------------------------
/// Do initialization of this context group
///
/// It is safe to call this function multiple times, successive calls will do nothing if the
/// context group is already initialized.
///
/// \warning The context passed inn \a currentContext must be the current context and it must
///          be a member of this context group.
//--------------------------------------------------------------------------------------------------
bool OpenGLContextGroup::initializeContextGroup(OpenGLContext* currentContext)
{
    //Trace::show("OpenGLContextGroup::initializeContextGroup()");

    CVF_ASSERT(currentContext);
    CVF_ASSERT(currentContext->isCurrent());
    CVF_ASSERT(containsContext(currentContext));

    if (!m_isInitialized)
    {
#ifdef CVF_USE_GLEW

        if (!initializeGLEW(currentContext))
        {
            CVF_LOG_ERROR(m_logger.p(), "Failed to intitialize GLEW in context group");
            return false;
        }

        configureCapabilitiesFromGLEW(currentContext);

#ifdef WIN32
        if (!initializeWGLEW(currentContext))
        {
            CVF_LOG_ERROR(m_logger.p(), "Failed to intitialize WGLEW in context group");
            return false;
        }
#endif

#endif

        CVF_LOG_DEBUG(m_logger.p(), "OpenGL initialized in context group");
        CVF_LOG_DEBUG(m_logger.p(), "  version:  " + m_info.version());
        CVF_LOG_DEBUG(m_logger.p(), "  vendor:   " + m_info.vendor());
        CVF_LOG_DEBUG(m_logger.p(), "  renderer: " + m_info.renderer());

        m_isInitialized = true;
    }

    return true;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void OpenGLContextGroup::uninitializeContextGroup()
{
    //Trace::show("OpenGLContextGroup::uninitializeContextGroup()");

    CVF_ASSERT(m_contexts.empty());
    CVF_ASSERT(!m_resourceManager->hasAnyOpenGLResources());

    // Just replace capabilities with a new object
    m_capabilities = new OpenGLCapabilities;

#ifdef CVF_USE_GLEW
    delete m_glewContextStruct;

#ifdef WIN32
    delete m_wglewContextStruct;
#endif
#endif
    m_glewContextStruct = NULL;
    m_wglewContextStruct = NULL;

    m_isInitialized = false;
}


//--------------------------------------------------------------------------------------------------
/// Prepare a context for deletion
///
/// This function will remove the context \a currentContextToShutdown from the context group.
/// If \a currentContextToShutdown is the last context in the group, all resources in the group's
/// resource manager will be deleted and the context group will be reset to uninitialized.
///
/// \warning  The passed context must be the current OpenGL context!
/// \warning  After calling this function the context is no longer usable and should be deleted.
//--------------------------------------------------------------------------------------------------
void OpenGLContextGroup::contextAboutToBeShutdown(OpenGLContext* currentContextToShutdown)
{
    //Trace::show("OpenGLContextGroup::contextAboutToBeShutdown()");

    CVF_ASSERT(currentContextToShutdown);
    CVF_ASSERT(containsContext(currentContextToShutdown));

    // If this is the last context in the group, we'll delete all the OpenGL resources in the s resource manager before we go
    bool shouldUninitializeGroup = false;
    if (contextCount() == 1)
    {
        CVF_ASSERT(m_resourceManager.notNull());
        if (m_resourceManager->hasAnyOpenGLResources() && currentContextToShutdown->isContextValid())
        {
            m_resourceManager->deleteAllOpenGLResources(currentContextToShutdown);
        }

        shouldUninitializeGroup = true;
    }

    // If the ref count on the context is down to 1, there is probably something strange going on.
    // Since one reference to the context is being held by this context group, this means that the
    // caller isn't holding a reference to the context. In this case the context object will evaporate 
    // during the call below, and the caller will most likely get a nasty surprise
    CVF_ASSERT(currentContextToShutdown->refCount() > 1);

    // Make sure we set the back pointer before removing it from our collection
    // since the removal from the list may actually trigger destruction of the context (see comment above)
    currentContextToShutdown->m_contextGroup = NULL;
    m_contexts.erase(currentContextToShutdown);

    if (shouldUninitializeGroup)
    {
        // Bring context group back to virgin state
        uninitializeContextGroup();
    }
}


//--------------------------------------------------------------------------------------------------
/// Initializes GLEW for this context group
///
/// \warning The context passed in \a currentContext must be current!
//--------------------------------------------------------------------------------------------------
bool OpenGLContextGroup::initializeGLEW(OpenGLContext* currentContext)
{
    //Trace::show("OpenGLContextGroup::initializeGLEW()");

    CVF_ASSERT(currentContext);
    CVF_ASSERT(m_glewContextStruct == NULL);

#ifdef CVF_USE_GLEW

    // Usage of GLEW requires that we have a standard OpenGL implementation available (not any Qt wrapper etc)
    // Supposedly the version string should always contain at least one '.'
    // Try and test this by querying the OpenGL version number and assume that there is no OpenGL available if it fails
    const String sVersion(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    if (sVersion.find(".") == String::npos)
    {
        CVF_LOG_ERROR(m_logger, "Error initializing OpenGL functions, probe for OpenGL version failed. No valid OpenGL context is current");
        return false;
    }

    // Since we're sometimes (when using Core OpenGL) seeing some OGL errors from GLEW, check before and after call to help find them
    CVF_CHECK_OGL(currentContext);

    // Must allocate memory for struct first since glewInit() call below will try and retrieve it
    GLEWContextStruct* theContextStruct = new GLEWContextStruct;
    memset(theContextStruct, 0, sizeof(GLEWContextStruct));
    GLenum err = glewContextInit(theContextStruct);
    if (err != GLEW_OK)
    {
        CVF_LOG_ERROR(m_logger, String("Error initializing GLEW, glewContextInit() returned %1").arg(err));
        delete theContextStruct;
        return false;
    }

    m_glewContextStruct = theContextStruct;

    CVF_CHECK_OGL(currentContext);

    String sVendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    String sRenderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    m_info.setOpenGLStrings(sVersion, sVendor, sRenderer);

#else

    CVF_FAIL_MSG("Not implemented");

#endif

    return true;
}


//--------------------------------------------------------------------------------------------------
/// Initializes GLEW for this context group
///
/// \warning The context passed in \a currentContext must be current!
//--------------------------------------------------------------------------------------------------
bool OpenGLContextGroup::initializeWGLEW(OpenGLContext* currentContext)
{
    CVF_ASSERT(currentContext);
    CVF_ASSERT(m_wglewContextStruct == NULL);

#if defined(CVF_USE_GLEW) && defined(WIN32)

    // Since we're sometimes (when using Core OpenGL) seeing some OGL errors from GLEW, check before and after call to help find them
    CVF_CHECK_OGL(currentContext);

    // Must allocate memory for struct first since glewInit() call below will try and retrieve it
    WGLEWContextStruct* theContextStruct = new WGLEWContextStruct;
    memset(theContextStruct, 0, sizeof(WGLEWContextStruct));
    GLenum err = wglewContextInit(theContextStruct);
    if (err != GLEW_OK)
    {
        delete theContextStruct;
        return false;
    }

    m_wglewContextStruct = theContextStruct;

    CVF_CHECK_OGL(currentContext);

#else

    CVF_FAIL_MSG("Not implemented");

#endif

    return true;
}


//--------------------------------------------------------------------------------------------------
/// Configures the capabilities of this context group by querying GLEW
///
/// \warning The passed context must be current and GLEW must already be initialized!
//--------------------------------------------------------------------------------------------------
void OpenGLContextGroup::configureCapabilitiesFromGLEW(OpenGLContext* currentContext)
{
#ifdef CVF_USE_GLEW
    CVF_CALLSITE_GLEW(currentContext);
    CVF_ASSERT(m_glewContextStruct);

    // Simplified config for now, we only differentiate between 1, 2, and 3
    uint majorVer = 1;
    if (GLEW_VERSION_2_0) majorVer = 2;
    if (GLEW_VERSION_3_0) majorVer = 3;
    m_capabilities->configureOpenGLSupport(majorVer);
    m_capabilities->setSupportsFixedFunction(true);


    if (GLEW_VERSION_3_0 || GLEW_ARB_framebuffer_object)
    {
        m_capabilities->addCapablity(OpenGLCapabilities::FRAMEBUFFER_OBJECT);
        m_capabilities->addCapablity(OpenGLCapabilities::GENERATE_MIPMAP_FUNC);
    }

    if (GLEW_VERSION_3_0 || GLEW_ARB_texture_float)
    {
        m_capabilities->addCapablity(OpenGLCapabilities::TEXTURE_FLOAT);
    }

    if (GLEW_VERSION_3_0 || GLEW_ARB_texture_rg)
    {
        m_capabilities->addCapablity(OpenGLCapabilities::TEXTURE_RG);
    }

    if (GLEW_VERSION_3_1 || GLEW_ARB_texture_rectangle)
    {
        m_capabilities->addCapablity(OpenGLCapabilities::TEXTURE_RECTANGLE);
    }
#endif
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
size_t OpenGLContextGroup::contextCount() const
{
    return m_contexts.size();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
cvf::OpenGLContext* OpenGLContextGroup::context(size_t index)
{
    return m_contexts.at(index);
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool OpenGLContextGroup::containsContext(const OpenGLContext* context) const
{
    if (m_contexts.contains(context))
    {
        CVF_ASSERT(context->group() == this);
        return true;
    }
    else
    {
        CVF_ASSERT(context->group() != this);
        return false;
    }
}


//--------------------------------------------------------------------------------------------------
/// Add an OpenGLContext to this group
/// 
/// \warning It is the caller's responsibility to ensure that the context being added is actually
///          capable of sharing OpenGL resources (display lists etc) with the contexts that are 
///          already in the group. This function will not do any verifications to assert whether this is true.
//--------------------------------------------------------------------------------------------------
void OpenGLContextGroup::addContext(OpenGLContext* contextToAdd)
{
    CVF_ASSERT(contextToAdd);

    // Illegal to add a context that is already in our group
    CVF_ASSERT(!m_contexts.contains(contextToAdd));

    // Guard against adding a context that is already member of a another group
    CVF_ASSERT(contextToAdd->group() == NULL || contextToAdd->group() == this);

    // Add it to our group and set ourselves as the owning group using our friend powers
    m_contexts.push_back(contextToAdd);
    contextToAdd->m_contextGroup = this;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
OpenGLResourceManager* OpenGLContextGroup::resourceManager()
{
    CVF_ASSERT(m_resourceManager.notNull());
    return m_resourceManager.p();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
Logger* OpenGLContextGroup::logger()
{
    return m_logger.p();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
OpenGLCapabilities* OpenGLContextGroup::capabilities()
{
    CVF_ASSERT(m_capabilities.notNull());
    return m_capabilities.p();
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
OpenGLInfo OpenGLContextGroup::info() const
{
    return m_info;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
GLEWContextStruct* OpenGLContextGroup::glewContextStruct()
{
#ifdef CVF_USE_GLEW

    CVF_TIGHT_ASSERT(this);
    CVF_TIGHT_ASSERT(m_glewContextStruct);
    return m_glewContextStruct;

#else

    CVF_FAIL_MSG("Not implemented");
    return NULL;

#endif

}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
WGLEWContextStruct* OpenGLContextGroup::wglewContextStruct()
{
#if defined(CVF_USE_GLEW) && defined(WIN32)

    CVF_TIGHT_ASSERT(this);
    CVF_TIGHT_ASSERT(m_wglewContextStruct);
    return m_wglewContextStruct;

#else

    CVF_FAIL_MSG("Not implemented");
    return NULL;

#endif
}


} // namespace cvf
