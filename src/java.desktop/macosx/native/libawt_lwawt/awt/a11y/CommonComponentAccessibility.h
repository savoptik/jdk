/*
 * Copyright (c) 2021, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#ifndef JAVA_COMPONENT_ACCESSIBILITY
#define JAVA_COMPONENT_ACCESSIBILITY

#include "jni.h"

#import <AppKit/AppKit.h>
#import "JavaAccessibilityUtilities.h"

// these constants are duplicated in CAccessibility.java
#define JAVA_AX_ALL_CHILDREN (-1)
#define JAVA_AX_SELECTED_CHILDREN (-2)
#define JAVA_AX_VISIBLE_CHILDREN (-3)
// If the value is >=0, it's an index

@interface CommonComponentAccessibility : NSAccessibilityElement {
    NSView *fView;
    NSObject *fParent;

    NSString *fNSRole;
    NSString *fJavaRole;

    jint fIndex;
    jobject fAccessible;
    jobject fComponent;

    NSMutableDictionary *fActions;
    NSMutableArray *fActionSElectors;
    NSObject *fActionsLOCK;
}

@property(readonly) NSArray *actionSelectores;

- (id)initWithParent:(NSObject*)parent withEnv:(JNIEnv *)env withAccessible:(jobject)accessible withIndex:(jint)index withView:(NSView *)view withJavaRole:(NSString *)javaRole;
- (void)unregisterFromCocoaAXSystem;
- (void)postValueChanged;
- (void)postSelectedTextChanged;
- (void)postSelectionChanged;
- (void)postTitleChanged;
- (BOOL)isEqual:(id)anObject;
- (BOOL)isAccessibleWithEnv:(JNIEnv *)env forAccessible:(jobject)accessible;

+ (void)postFocusChanged:(id)message;

+ (void) initializeRolesMap;
+ (CommonComponentAccessibility * _Nullable) getComponentAccessibility:(NSString * _Nonnull)role;
+ (CommonComponentAccessibility *) getComponentAccessibility:(NSString *)role andParent:(CommonComponentAccessibility *)parent;

+ (NSArray*)childrenOfParent:(CommonComponentAccessibility *)parent withEnv:(JNIEnv *)env withChildrenCode:(NSInteger)whichChildren allowIgnored:(BOOL)allowIgnored;
+ (NSArray *)childrenOfParent:(CommonComponentAccessibility *) parent withEnv:(JNIEnv *)env withChildrenCode:(NSInteger)whichChildren allowIgnored:(BOOL)allowIgnored recursive:(BOOL)recursive;
+ (CommonComponentAccessibility *) createWithParent:(CommonComponentAccessibility *)parent accessible:(jobject)jaccessible role:(NSString *)javaRole index:(jint)index withEnv:(JNIEnv *)env withView:(NSView *)view;
+ (CommonComponentAccessibility *) createWithAccessible:(jobject)jaccessible role:(NSString *)role index:(jint)index withEnv:(JNIEnv *)env withView:(NSView *)view;
+ (CommonComponentAccessibility *) createWithAccessible:(jobject)jaccessible withEnv:(JNIEnv *)env withView:(NSView *)view;

// If the isWraped parameter is true, then the object passed as a parent was created based on the same java component,
// but performs a different NSAccessibilityRole of a table cell, or a list row, or tree row,
// and we need to create an element whose role corresponds to the role in Java.
+ (CommonComponentAccessibility *) createWithParent:(CommonComponentAccessibility *)parent accessible:(jobject)jaccessible role:(NSString *)javaRole index:(jint)index withEnv:(JNIEnv *)env withView:(NSView *)view isWrapped:(BOOL)wrapped;

// The current parameter is used to bypass the check for an item's index on the parent so that the item is created. This is necessary,
// for example, for AccessibleJTreeNode, whose currentComponent has index -1
+ (CommonComponentAccessibility *) createWithAccessible:(jobject)jaccessible withEnv:(JNIEnv *)env withView:(NSView *)view isCurrent:(BOOL)current;

- (jobject)axContextWithEnv:(JNIEnv *)env;
- (NSView*)view;
- (NSWindow*)window;
- (id)parent;
- (NSString *)javaRole;

- (BOOL)isMenu;
- (BOOL)isSelected:(JNIEnv *)env;
- (BOOL)isSelectable:(JNIEnv *)env;
- (BOOL)isVisible:(JNIEnv *)env;

- (NSArray *)accessibleChildrenWithChildCode:(NSInteger)childCode;

- (NSDictionary*)getActions:(JNIEnv *)env;
- (void)getActionsWithEnv:(JNIEnv *)env;
- (BOOL)accessiblePerformAction:(NSAccessibilityActionName)actionName;
- (BOOL)performAccessibleAction:(int)index;

- (NSRect)accessibilityFrame;
- (nullable id)accessibilityParent;
- (BOOL)isAccessibilityElement;
@end

#endif
