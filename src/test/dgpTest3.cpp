//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-05 23:18:19 taubin>
//------------------------------------------------------------------------
//
// dgpTest3.cpp
//
// Software developed for course
// Digital Geometry Processing
// Copyright (c) 2025, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <string>
#include <iostream>

using namespace std;

#include <wrl/SceneGraphTraversal.hpp>

#include <io/AppLoader.hpp>
#include <io/AppSaver.hpp>
#include <io/LoaderPly.hpp>
#include <io/LoaderStl.hpp>
#include <io/LoaderWrl.hpp>
#include <io/SaverPly.hpp>
#include <io/SaverStl.hpp>
#include <io/SaverWrl.hpp>

#include <core/PolygonMesh.hpp>
#include <core/PolygonMeshTest.hpp>

#include "dgpPrt.hpp"

enum Operation {
  NONE,
  COMPUTE_CC_PRIMAL,
  COMPUTE_CC_DUAL,
  IS_ORIENTED,
  IS_ORIENTABLE,
  ORIENT,
  REMOVE_ISOLATED_VERTICES,
  CUT_THROUGH_SINGULAR_VERTICES,
  CONVERT_TO_MANIFOLD
};

// TODO Mon Mar 6 2023
// - add code to test

// PolygonMesh::computeConnectedComponentsPrimal()
// PolygonMesh::computeConnectedComponentsDual()
// PolygonMesh::isOriented()
// PolygonMesh::isOrientable()
// PolygonMesh::orient()
// PolygonMesh::removeIsolatedVertices()
// PolygonMesh::cutThroughSingularVertices()
// PolygonMesh::convertToManifold()

class Data {
public:
  bool   _debug;
  bool   _binaryOutput;
  bool   _removeProperties;

  // TODO Mon Mar 6 2023
  // - add variables to specify the operation to be performed
  Operation _operation;

  string _inFile;
  string _outFile;
public:
  Data():
    _debug(false),
    _binaryOutput(false),
    _removeProperties(false),
    _operation(NONE),
    _inFile(""),
    _outFile("")
  { }
};

void options(Data& D) {
  cout << "   -d|-debug               [" << tv(D._debug)            << "]" << endl;
  cout << "   -b|-binaryOutput        [" << tv(D._binaryOutput)     << "]" << endl;
  cout << "   -r|-removeProperties    [" << tv(D._removeProperties) << "]" << endl;

  // TODO Mon Mar 6 2023
  // - add line(s) to explain how to specify the operation to be performed

  cout << " -ccp | -ccPrimal ["
       << tv(D._operation==Operation::COMPUTE_CC_PRIMAL) << "]" << endl;
}

void usage(Data& D) {
  cout << "USAGE: dgpTest3 [options] inFile outFile" << endl;
  cout << "   -h|-help" << endl;
  options(D);
  cout << endl;
  exit(0);
}

void error(const char *msg) {
  cout << "ERROR: dgpTest3 | " << ((msg)?msg:"") << endl;
  exit(0);
}

//////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {

  Data D;

  if(argc==1) usage(D);

  for(int i=1;i<argc;i++) {
    if(string(argv[i])=="-h" || string(argv[i])=="-help") {
      usage(D);
    } else if(string(argv[i])=="-d" || string(argv[i])=="-debug") {
      D._debug = !D._debug;
    } else if(string(argv[i])=="-b" || string(argv[i])=="-binaryOutput") {
      D._binaryOutput = !D._binaryOutput;
    } else if(string(argv[i])=="-r" || string(argv[i])=="-removeProperties") {
      D._removeProperties = !D._removeProperties;

      // TODO Mon Mar 6 2023
      // - add code to parse the desired operation to be performed
      // - from the command line

    } else if(string(argv[i])=="-ccp" || string(argv[i])=="-ccPrimal") {
      D._operation = Operation::COMPUTE_CC_PRIMAL;
    } else if(string(argv[i])=="-ccd" || string(argv[i])=="-ccDual") {
      D._operation = Operation::COMPUTE_CC_DUAL;
    } else if(string(argv[i])=="-iso" || string(argv[i])=="-isOriented") {
      D._operation = Operation::IS_ORIENTED;
    } else if(string(argv[i])=="-isot" || string(argv[i])=="-isOrientable") {
      D._operation = Operation::IS_ORIENTABLE;
    } else if(string(argv[i])=="-or" || string(argv[i])=="-orient") {
      D._operation = Operation::ORIENT;
    } else if(string(argv[i])=="-riv" || string(argv[i])=="-removeIsolatedVertices") {
      D._operation = Operation::REMOVE_ISOLATED_VERTICES;
    } else if(string(argv[i])=="-ctsv" || string(argv[i])=="-cutThroughSingularVertices") {
      D._operation = Operation::CUT_THROUGH_SINGULAR_VERTICES;
    } else if(string(argv[i])=="-ctm" || string(argv[i])=="-convertToManifold") {
      D._operation = Operation::CONVERT_TO_MANIFOLD;

    } else if(string(argv[i])[0]=='-') {
      error("unknown option");
    } else if(D._inFile=="") {
      D._inFile = string(argv[i]);
    } else if(D._outFile=="") {
      D._outFile = string(argv[i]);
    }
  }

  if(D._inFile =="") error("no inFile");

  // if D._outFile is not specified then no output file will be written
  // if(D._outFile=="") error("no outFile");

  if(D._debug) {
    cout << "dgpTest3 {" << endl;
    cout << endl;
    options(D);
    cout << endl;
    cout << "  inFile  = " << D._inFile << endl;
    cout << "  outFile = " << D._outFile << endl;
    cout << endl;
    fflush(stderr);
  }

  bool success;

  //////////////////////////////////////////////////////////////////////
  // create loader and saver factories
  AppLoader loaderFactory;
  AppSaver  saverFactory;

  // register input file loaders
  LoaderPly* plyLoader = new LoaderPly();
  loaderFactory.registerLoader(plyLoader);
  LoaderStl* stlLoader = new LoaderStl();
  loaderFactory.registerLoader(stlLoader);
  LoaderWrl* wrlLoader = new LoaderWrl();
  loaderFactory.registerLoader(wrlLoader);

  //  If SaverPly::setDefaultDataType is used, it must be called
  //  before the Saver constructor; otherwise SaverPly::setDataType
  //  should be called after to set the proper value for the private
  //  variable SaverPly::_dataType before this instance of SaverPly is
  //  used

  // register output file savers  
  SaverPly* plySaver = new SaverPly();
  saverFactory.registerSaver(plySaver);
  SaverStl* stlSaver = new SaverStl();
  saverFactory.registerSaver(stlSaver);
  SaverWrl* wrlSaver = new SaverWrl();
  saverFactory.registerSaver(wrlSaver);

  SaverStl::FileType stlFt =
    (D._binaryOutput)?SaverStl::FileType::BINARY:SaverStl::FileType::ASCII;
  stlSaver->setFileType(stlFt);

  Ply::DataType plyDt =
    (D._binaryOutput)?Ply::DataType::BINARY_LITTLE_ENDIAN:Ply::DataType::ASCII;
  plySaver->setDataType(plyDt);

  if(D._debug) {
    SaverPly::setOstream(&cout);
    SaverPly::setIndent("    ");
  }

  //////////////////////////////////////////////////////////////////////
  // read SceneGraph

  SceneGraph wrl; // create empty scene graph

  if(D._debug) {
    cout << "  loading inFile {" << endl;
  }

  success = loaderFactory.load(D._inFile.c_str(),wrl);

  if(D._debug) {
    cout << "    success        = " << tv(success)          << endl;
    cout << "  } loading inFile" << endl;
    cout << endl;
  }

  if(success==false) return -1;

  if(D._removeProperties) {
    if(D._debug) cout << "  removing properties {" << endl;
    Node* node;
    SceneGraphTraversal sgt(wrl);
    for(int iIfs=0;(node=sgt.next())!=(Node*)0;iIfs++) {
      Shape* shape = dynamic_cast<Shape*>(node);
      if(shape==(Shape*)0) continue;
      IndexedFaceSet* ifs = dynamic_cast<IndexedFaceSet*>(shape->getGeometry());
      if(ifs==(IndexedFaceSet*)0) continue;
      ifs->setNormalPerVertex(true);
      ifs->getNormal().clear();
      ifs->getNormalIndex().clear();
      ifs->setColorPerVertex(true);
      ifs->getColor().clear();
      ifs->getColorIndex().clear();
      ifs->getTexCoord().clear();
      ifs->getTexCoordIndex().clear();
    }
    if(D._debug) cout << "  } removing properties" << endl;
    if(D._debug) cout << endl;
  }

  // print PolygonMesh info before processing
  if(D._debug) {
    cout << "  before processing" << endl;
    PolygonMeshTest(wrl,"  ");
    cout << endl;
  }

  // process
  
  if(D._debug) cout << "  processing {" << endl;

  // TODO Mon Mar 6 2023
  // - add code to perform the desired operation
  // - for each case print some information about the results

  // PolygonMesh::computeConnectedComponentsPrimal()
  // PolygonMesh::computeConnectedComponentsDual()
  // PolygonMesh::isOriented()
  // PolygonMesh::isOrientable()
  // PolygonMesh::orient()
  // PolygonMesh::removeIsolatedVertices()
  // PolygonMesh::cutThroughSingularVertices()
  // PolygonMesh::convertToManifold()

  Node* node;
  SceneGraphTraversal sgt(wrl);
  for(int iIfs=0;(node=sgt.next())!=(Node*)0;iIfs++) {
    Shape* shape = dynamic_cast<Shape*>(node);
    if(shape==(Shape*)0) continue;
    IndexedFaceSet* ifs = dynamic_cast<IndexedFaceSet*>(shape->getGeometry());
    if(ifs==(IndexedFaceSet*)0) continue;

    // Perform the operation on each of the IndexedFaceSet nodes
    // First I create its PolygonMesh
    int nVifs = ifs->getNumberOfCoord();
    vector<int>& coordIndex = ifs->getCoordIndex();


    PolygonMesh pMesh(nVifs, coordIndex);

    
    cout << "OPERATION" << endl;
    switch(D._operation) {
    case Operation::COMPUTE_CC_PRIMAL: {
      vector<int> faceLabel;
      int nCC = pMesh.computeConnectedComponentsPrimal(faceLabel);

      // Get CC size
      map<int,int> ccSize;
      for (size_t i=0; i < faceLabel.size(); i++) {
          ccSize[faceLabel[i]]++;
      }
      cout << endl;
      cout << " COMPUTE_CC_PRIMAL:" << endl;
      cout << "    IndexedFaceSet " << iIfs << " | "
           << " nCC = " << nCC << endl;
      cout << "      CC sizes: ";
      for (const auto& pair : ccSize) {
          cout << "       CC " << pair.first << " size = " << pair.second << "; ";
      }
      cout << endl << endl;
      break;
    }
    case Operation::COMPUTE_CC_DUAL: {
      vector<int> faceLabel;
      int nCC = pMesh.computeConnectedComponentsDual(faceLabel);

      // Get CC size
      map<int,int> ccSize;
      for (size_t i=0; i < faceLabel.size(); i++) {
          ccSize[faceLabel[i]]++;
      }
      cout << endl;
      cout << " COMPUTE_CC_DUAL:" << endl;
      cout << "    IndexedFaceSet " << iIfs << " | "
           << " nCC = " << nCC << endl;
      cout << "      CC sizes: ";
      for (const auto& pair : ccSize) {
          cout << "       CC " << pair.first << " size = " << pair.second << "; ";
      }
      cout << endl << endl;
      break;
    }
    case Operation::IS_ORIENTED: {
      bool isOriented = pMesh.isOriented();
      cout << endl;
      cout << " IS_ORIENTED:" << endl;
      cout << "    IndexedFaceSet " << iIfs << " | "
            << " isOriented = " << tv(isOriented) << endl << endl;
      break;
    }
    case Operation::IS_ORIENTABLE: {
      bool isOrientable = pMesh.isOrientable();
      cout << endl;
      cout << " IS_ORIENTABLE:" << endl;
      cout << "    IndexedFaceSet " << iIfs << " | "
            << " isOrientable = " << tv(isOrientable) << endl << endl;
      break;
    }
    case Operation::ORIENT: {
      vector<int> ccIndex;
      vector<bool> invertFace;
      pMesh.orient(ccIndex, invertFace);
      if (ccIndex.size() == 0) {
          cout << endl;
          cout << " ORIENT:" << endl;
          cout << "    IndexedFaceSet " << iIfs << " is already oriented or not orientable." << endl << endl;
          break;
      }
      cout << endl;
      cout << " ORIENT:" << endl;
      cout << "    IndexedFaceSet " << iIfs << " possible orientation:" << endl;
      cout << "      CC index: " << endl;
      for (size_t i = 0; i < ccIndex.size(); i++) {
          cout << "       Face " << i << " CC " << ccIndex[i] << "; ";
          if (invertFace[i])
            cout << " should be inverted." << endl;
          else
            cout << " should NOT be inverted." << endl;
      }
      cout << endl << endl;
      break;
    }
    case Operation::REMOVE_ISOLATED_VERTICES: {
      vector<int> coordMap;
      vector<int> coordIndexOut;
      bool result = pMesh.removeIsolatedVertices(coordMap, coordIndexOut);
      cout << endl;
      cout << " REMOVE_ISOLATED_VERTICES:" << endl;
      if (!result) {
          cout << "    IndexedFaceSet " << iIfs << " had no isolated vertices." << endl << endl;
          break;
      }
      cout << "    IndexedFaceSet " << iIfs << " had " << nVifs - coordMap.size() << " isolated vertices." << endl << endl;
      break;
    }
    case Operation::CUT_THROUGH_SINGULAR_VERTICES: {
      vector<int> vIndexMap;
      vector<int> coordIndexOut;
      pMesh.cutThroughSingularVertices(vIndexMap, coordIndexOut);
      cout << endl;
      cout << " CUT_THROUGH_SINGULAR_VERTICES:" << endl;
      cout << "    Mapping of new vertices:" << endl;
      for (size_t i = 0; i < vIndexMap.size(); i++) {
          cout << "       Old Vertex " << vIndexMap[i] << " -> New Vertex " << i << endl << endl;
      }
      break;
    }
    case Operation::CONVERT_TO_MANIFOLD: {
      vector<int> vIndexMap;
      vector<int> coordIndexOut;
      pMesh.convertToManifold(vIndexMap, coordIndexOut);
      cout << endl;
      cout << " CONVERT_TO_MANIFOLD:" << endl;
      cout << "    Mapping of new vertices:" << endl;
      for (size_t i = 0; i < vIndexMap.size(); i++) {
          cout << "       Old Vertex " << vIndexMap[i] << " -> New Vertex " << i << endl << endl;
      }
      break;
    }
    case Operation::NONE:
    default:
      break;
    }
  }


  if(D._debug) cout << "  } processing" << endl;
  
  // print PolygonMesh info after processing
  if(D._debug) {
    cout << "  after processing" << endl;
    PolygonMeshTest(wrl,"  ");
    cout << endl;
  }

  // write output file
  if(D._outFile!="") {

    if(D._debug) {
      cout << "  saving outFile {" << endl;
    }
        
    success = saverFactory.save(D._outFile.c_str(),wrl);
        
    if(D._debug) {
      cout << "    success        = " << tv(success)          << endl;
      cout << "  }" << endl;
      cout << endl;
    }

  } else {
    if(D._debug) {
      cout << "  no outFile written" << endl;
    }
  }
    
  //////////////////////////////////////////////////////////////////////

  if(D._debug) {
    cout << "} dgpTest3" << endl;
    fflush(stderr);
  }

  return 0;
}
