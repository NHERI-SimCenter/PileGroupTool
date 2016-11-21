
SOURCES += ./ops/Domain.cpp
SOURCES += ./ops/Subdomain.cpp
SOURCES += ./ops/DomainComponent.cpp
SOURCES += ./ops/ArrayOfTaggedObjects.cpp
SOURCES += ./ops/ArrayOfTaggedObjectsIter.cpp
SOURCES += ./ops/MapOfTaggedObjects.cpp
SOURCES += ./ops/MapOfTaggedObjectsIter.cpp
SOURCES += ./ops/SingleDomSP_Iter.cpp
SOURCES += ./ops/SingleDomMP_Iter.cpp
SOURCES += ./ops/SingleDomNodIter.cpp
SOURCES += ./ops/SubdomainNodIter.cpp
SOURCES += ./ops/SingleDomEleIter.cpp
SOURCES += ./ops/SingleDomParamIter.cpp
SOURCES += ./ops/SingleDomAllSP_Iter.cpp
SOURCES += ./ops/SingleDomPC_Iter.cpp
SOURCES += ./ops/LoadPatternIter.cpp
SOURCES += ./ops/NodalLoadIter.cpp
SOURCES += ./ops/FE_EleIter.cpp
SOURCES += ./ops/DOF_GrpIter.cpp
SOURCES += ./ops/ElementalLoadIter.cpp
SOURCES += ./ops/SP_Constraint.cpp
SOURCES += ./ops/MP_Constraint.cpp
SOURCES += ./ops/TaggedObject.cpp
SOURCES += ./ops/ZeroLength.cpp
SOURCES += ./ops/Element.cpp
SOURCES += ./ops/Information.cpp
SOURCES += ./ops/ElasticSection3d.cpp
SOURCES += ./ops/SectionForceDeformation.cpp
SOURCES += ./ops/LinearSeries.cpp
SOURCES += ./ops/TimeSeries.cpp
SOURCES += ./ops/MovableObject.cpp
SOURCES += ./ops/Channel.cpp
SOURCES += ./ops/Node.cpp
SOURCES += ./ops/NodalLoad.cpp
SOURCES += ./ops/Load.cpp
SOURCES += ./ops/LoadPattern.cpp
SOURCES += ./ops/DispBeamColumn3d.cpp
SOURCES += ./ops/CrdTransf.cpp
SOURCES += ./ops/LinearCrdTransf3d.cpp
SOURCES += ./ops/LegendreBeamIntegration.cpp
SOURCES += ./ops/BeamIntegration.cpp
SOURCES += ./ops/Response.cpp
SOURCES += ./ops/ElementResponse.cpp
SOURCES += ./ops/MaterialResponse.cpp
SOURCES += ./ops/CompositeResponse.cpp
SOURCES += ./ops/Matrix.cpp
SOURCES += ./ops/MatrixUtil.cpp
SOURCES += ./ops/ID.cpp
SOURCES += ./ops/Vector.cpp
SOURCES += ./ops/Graph.cpp
SOURCES += ./ops/Vertex.cpp
SOURCES += ./ops/VertexIter.cpp
SOURCES += ./ops/RCM.cpp
SOURCES += ./ops/GraphNumberer.cpp
SOURCES += ./ops/LoadControl.cpp
SOURCES += ./ops/PenaltyConstraintHandler.cpp
SOURCES += ./ops/TransformationConstraintHandler.cpp
SOURCES += ./ops/TransformationDOF_Group.cpp
SOURCES += ./ops/TransformationFE.cpp
SOURCES += ./ops/PenaltySP_FE.cpp
SOURCES += ./ops/PenaltyMP_FE.cpp
SOURCES += ./ops/ConstraintHandler.cpp
SOURCES += ./ops/IncrementalIntegrator.cpp
SOURCES += ./ops/StaticIntegrator.cpp
SOURCES += ./ops/Integrator.cpp
SOURCES += ./ops/PlainNumberer.cpp
SOURCES += ./ops/DOF_Numberer.cpp
SOURCES += ./ops/StaticAnalysis.cpp
SOURCES += ./ops/Analysis.cpp
SOURCES += ./ops/NewtonRaphson.cpp
SOURCES += ./ops/EquiSolnAlgo.cpp
SOURCES += ./ops/SolutionAlgorithm.cpp
SOURCES += ./ops/AnalysisModel.cpp
SOURCES += ./ops/CTestNormDispIncr.cpp
SOURCES += ./ops/ConvergenceTest.cpp
SOURCES += ./ops/LinearSOE.cpp
SOURCES += ./ops/LinearSOESolver.cpp
SOURCES += ./ops/BandGenLinSOE.cpp
SOURCES += ./ops/BandGenLinSolver.cpp
SOURCES += ./ops/BandGenLinLapackSolver.cpp
SOURCES += ./ops/FE_Element.cpp
SOURCES += ./ops/DOF_Group.cpp
SOURCES += ./ops/PySimple1.cpp
SOURCES += ./ops/TzSimple1.cpp
SOURCES += ./ops/QzSimple1.cpp
SOURCES += ./ops/UniaxialMaterial.cpp
SOURCES += ./ops/Material.cpp
SOURCES += ./ops/StandardStream.cpp
SOURCES += ./ops/DummyStream.cpp
SOURCES += ./ops/DummyElementAPI.cpp
SOURCES += ./ops/OPS_Stream.cpp


DEFINES += _bool_h

unix {

macx {

DEFINES += "_MacOSX"
LIBS += /Users/fmk/lib/libLapack.a
LIBS += /Users/fmk/lib/libBlas.a
LIBS += /usr/local/llvm-gcc/lib/libgfortran.a


INCLUDEPATH += .include "./ops"
#INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"

} else {


DEFINES += "_LINUX"


INCLUDEPATH += "/apps/rappture/dev/include"
INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"

SRC += ./ops/Domain.cpp

}
}

win32 {
INCLUDEPATH += C:\Progra~1\Tcl\include
INCLUDEPATH += $$(HOME)\OpenSees\DEVELOPER\core

# NOTE THAT THE OpenSees libs have to be created with /MD as opposed to /MT as runtime library
# this is specified in C++ -> Code Generation -> RunTime Library
# this is because Qt must be built with this option as they have memory issues if built /MT


CONFIG += static

QMAKE_LFLAGS_DEBUG += /FORCE:MULTIPLE
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:"libc.lib"
QMAKE_LFLAGS_RELEASE += /FORCE:MULTIPLE
QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:"libc.lib"

QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:"libcmt.lib"
LIBPATH += "C:\Program Files (x86)\Intel\Composer XE\compiler\lib\intel64"


} else {

}


               
