/**
 * Copyright (c) 2015-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>

#include <folly/dynamic.h>

#include <fabric/core/ShadowNode.h>
#include <fabric/uimanager/ComponentDescriptorRegistry.h>
#include <fabric/uimanager/UIManagerDelegate.h>

namespace facebook {
namespace react {

using CreateEventTargetFunction = void *(void *instanceHandle);
using DispatchEventFunction = void (void *eventHandler, void *eventTarget, std::string type, folly::dynamic payload);
using ReleaseEventTargetFunction = void (void *eventTarget);
using ReleaseEventHandlerFunction = void (void *eventHandler);

class FabricUIManager {
public:

#pragma mark - Native-facing Interface

  FabricUIManager(SharedComponentDescriptorRegistry componentDescriptorRegistry);
  ~FabricUIManager();

  /*
   * Sets and gets the UIManager's delegate.
   * The delegate is stored as a raw pointer, so the owner must null
   * the pointer before being destroyed.
   */
  void setDelegate(UIManagerDelegate *delegate);
  UIManagerDelegate *getDelegate();

#pragma mark - Callback Functions

  /*
   * Registers callback functions.
   */
  void setCreateEventTargetFunction(std::function<CreateEventTargetFunction> createEventTargetFunction);
  void setDispatchEventFunction(std::function<DispatchEventFunction> dispatchEventFunction);
  void setReleaseEventTargetFunction(std::function<ReleaseEventTargetFunction> releaseEventTargetFunction);
  void setReleaseEventHandlerFunction(std::function<ReleaseEventHandlerFunction> releaseEventHandlerFunction);

#pragma mark - Native-facing Interface

  void *createEventTarget(void *instanceHandle);
  void dispatchEvent(void *eventTarget, const std::string &type, const folly::dynamic &payload);
  void releaseEventTarget(void *eventTarget);

#pragma mark - JavaScript/React-facing Interface

  SharedShadowNode createNode(Tag reactTag, std::string viewName, Tag rootTag, folly::dynamic props, InstanceHandle instanceHandle);
  SharedShadowNode cloneNode(const SharedShadowNode &node, InstanceHandle instanceHandle);
  SharedShadowNode cloneNodeWithNewChildren(const SharedShadowNode &node, InstanceHandle instanceHandle);
  SharedShadowNode cloneNodeWithNewProps(const SharedShadowNode &node, folly::dynamic props, InstanceHandle instanceHandle);
  SharedShadowNode cloneNodeWithNewChildrenAndProps(const SharedShadowNode &node, folly::dynamic newProps, InstanceHandle instanceHandle);
  void appendChild(const SharedShadowNode &parentNode, const SharedShadowNode &childNode);
  SharedShadowNodeUnsharedList createChildSet(Tag rootTag);
  void appendChildToSet(const SharedShadowNodeUnsharedList &childSet, const SharedShadowNode &childNode);
  void completeRoot(Tag rootTag, const SharedShadowNodeUnsharedList &childSet);
  void registerEventHandler(void *eventHandler);

private:

  SharedComponentDescriptorRegistry componentDescriptorRegistry_;
  UIManagerDelegate *delegate_;
  void *eventHandler_;
  std::function<CreateEventTargetFunction> createEventTargetFunction_;
  std::function<DispatchEventFunction> dispatchEventFunction_;
  std::function<ReleaseEventTargetFunction> releaseEventTargetFunction_;
  std::function<ReleaseEventHandlerFunction> releaseEventHandlerFunction_;
};

} // namespace react
} // namespace facebook
