/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    genTclGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef _genTclGenerator_h
#define _genTclGenerator_h

#include "genUtils.h"
#include "genGeneratorBase.h"
#include "cableSourceRepresentation.h"
#include "genCvTypeGenerator.h"

namespace gen
{


/**
 * Generation class for Tcl wrappers.
 */
class GENERATORS_EXPORT TclGenerator: public GeneratorBase
{
public:
  TclGenerator(const configuration::CableConfiguration*,
               const source::Namespace*, std::ostream&);
  virtual ~TclGenerator() {}
  
  static GeneratorBase* GetInstance(const configuration::CableConfiguration*,
                                    const source::Namespace*, std::ostream&);  
  
  virtual void Generate();  
private:
  class MethodEntry
  {
  public:
    MethodEntry(source::Method* method, unsigned int argc):
      m_Method(method), m_ArgumentCount(argc) {}
    unsigned int GetArgumentCount() const { return m_ArgumentCount; }
    source::Method* operator->() const { return m_Method; }
    operator source::Method*() const { return m_Method; }
  private:
    source::Method* m_Method;
    unsigned int m_ArgumentCount;
  };
  
  typedef std::vector<String> WrapperList;
  typedef std::vector<MethodEntry> Methods;
  
  void GenerateWrappers();
  void GeneratePackageInitializer();
  void GenerateNamespace(const configuration::Namespace*);
  void GenerateClassWrapper(const source::Class*, const configuration::Class*);
  bool ReturnsVoid(const source::Function*) const;
  void WriteConverterRegistration(const String&, const MethodEntry&) const;
  void WriteEnumValueRegistration() const;
  void WriteMethodRegistration(const String&, const MethodEntry&,
                               unsigned int) const;
  void WriteWrapperClassDefinition(const source::Class*, const Methods&,
                                   const configuration::Class*) const;
  void WriteImplicitArgument(const source::Class*, const source::Method*) const;
  void WriteArgumentList(const source::ArgumentContainer&, unsigned int, unsigned int) const;
  void WriteReturnBegin(const source::Function*) const;
  void WriteReturnEnd(const source::Function*) const;
  void WriteMethodComment(const String&, const MethodEntry&) const;
  void WriteConversionInititialization() const;
  void WriteReturnEnumClasses() const;
  void FindCvTypes(const configuration::Namespace*);
  void FindCvTypes(const source::Class*);
  void FindCvTypes(const source::Method*);
  void AddSuperclassCvTypes(const cxx::ClassType*);
  cxx::CvQualifiedType GetCxxType(const source::Type*) const;

  /**
   * Generator to write out CxxType representation construction code.
   */
  CvTypeGenerator  m_CvTypeGenerator;
  
  WrapperList m_WrapperList;
  
  typedef std::set<const cxx::ClassType*> ClassesForDerivedToBase;
  ClassesForDerivedToBase m_ClassesForDerivedToBase;

  typedef std::set<const cxx::ClassType*> ClassesThatNeedDestructor;
  ClassesThatNeedDestructor m_ClassesThatNeedDestructor;

  typedef std::set<const source::Enumeration*> EnumTypesThatNeedReturn;
  EnumTypesThatNeedReturn m_EnumTypesThatNeedReturn;

  typedef std::set<const source::Enumeration*> EnumTypesThatNeedValues;
  EnumTypesThatNeedValues m_EnumTypesThatNeedValues;
};

} // namespace gen
  
#endif
