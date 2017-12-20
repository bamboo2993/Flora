#include "CMix.h"
#include "common\CBag.h"

using namespace cocos2d;

#define DATA_ROOT_NAME    "DataRoot"
#define XML_FILE_NAME "Data.xml"
CMix::CMix(cocos2d::Node *currentNode) {
	_Node = currentNode;

	//_xmlbag = new xmlBag("./res/xml/xmlfile_bag.xml");
	//_xmlitem = new xmlItem();
	_xmlscene = new xmlScene("./res/xml/xmlfile_BMScene.xml");

	_ioriginalItem = 0;
	//_itargetNum = 0;
	_count = 0;
	ir[0] = 0;
	ir[1] = 0;
	ir[2] = 0;
	ir[3] = 0;
	ir[4] = 0;
	_bstate = false;



	for (size_t i = 0; i < 4; i++){
		_targetItem1[i] = nullptr;
		_targetItem2[i] = nullptr;
		_targetItem3[i] = nullptr;
		_targetItem4[i] = nullptr;
		_targetItem5[i] = nullptr;


	}

	for (size_t i = 0; i < 2; i++) {
		_result1[i] = 0;
		_result2[i] = 0;
		_result3[i] = 0;
		_result4[i] = 0;
		_result5[i] = 0;


	}

	for (size_t i = 0; i < 7; i++) {
		_mixItem[i] = nullptr;
	}

	
	_itargetNum[0] = 0;
	_itargetNum[1] = 0;
	_itargetNum[2] = 0;
	_itargetNum[3] = 0;
	_itargetNum[4] = 0;

	_step = 0;
	_currentShown = -1;
	_currentSteps = 0;
}

CMix::~CMix() {


}

void CMix::init(int original, int failure, bool type) {
	_ioriginalItem = original;
	_ifailureItem = failure;
	_currentShown = _ioriginalItem;

	_btype = type;
}
//
//void CMix::SetCombination(int combiA, int combiB, int result) {
//	_targetItem[_itargetNum] = _xmlscene->getBItemNameXML(combiA);
//	_itargetNum++;
//
//	_targetItem[_itargetNum] = _xmlscene->getBItemNameXML(combiB);
//	_itargetNum++;
//
//	_result[ir] = result;
//	ir++;
//
//}



void CMix::SetCombination(int combiA, bool typeA, int combiB, bool typeB, int result, int step) {
	auto steps = step-1;
	if (step == 1) {
		if (typeA) {
			_target1[_itargetNum[steps]] = combiA;
			_targetItem1[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiA);
			_itargetNum[steps]++;
		}
		else {
			_target1[_itargetNum[steps]] = combiA;
			_targetItem1[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiA);
			_itargetNum[steps]++;
		}

		if (typeB) {
			_target1[_itargetNum[steps]] = combiB;
			_targetItem1[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiB);
			_itargetNum[steps]++;
		}

		else {
			_target1[_itargetNum[steps]] = combiB;
			_targetItem1[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiB);
			_itargetNum[steps]++;
		}

		_result1[ir[0]] = result;
		//_combiNum[ir] = step;
		ir[0]++;

		if (_step < step)_step = step;
	}
	else if (step == 2) {
		if (typeA) {
			_target2[_itargetNum[steps]] = combiA;
			_targetItem2[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiA);
			_itargetNum[steps]++;

		}
		else {
			_target2[_itargetNum[steps]] = combiA;
			_targetItem2[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiA);
			_itargetNum[steps]++;

		}

		if (typeB) {

			_target2[_itargetNum[steps]] = combiB;
			_targetItem2[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiB);
			_itargetNum[steps]++;
		}
		else {
			_target2[_itargetNum[steps]] = combiB;
			_targetItem2[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiB);
			_itargetNum[steps]++;
		}


		_result2[ir[1]] = result;

		ir[1]++;
		if (_step < step)_step = step;
	}
	else if (step == 3) {

		if (typeA) {

			_target3[_itargetNum[steps]] = combiA;
			_targetItem3[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiA);
			_itargetNum[steps]++;
		}
		else {

			_target3[_itargetNum[steps]] = combiA;
			_targetItem3[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiA);
			_itargetNum[steps]++;
		}

		if (typeB) {
			_target3[_itargetNum[steps]] = combiB;
			_targetItem3[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiB);
			_itargetNum[steps]++;
		}
		else {
			_target3[_itargetNum[steps]] = combiB;
			_targetItem3[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiB);
			_itargetNum[steps]++;
		}


		_result3[ir[2]] = result;

		ir[2]++;
		if (_step < step)_step = step;
	}

	else if (step == 4) {

		if (typeA) {
			_target4[_itargetNum[steps]] = combiA;
			_targetItem4[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiA);
			_itargetNum[steps]++;
		}
		else {
			_target4[_itargetNum[steps]] = combiA;
			_targetItem4[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiA);
			_itargetNum[steps]++;
		}

		if (typeB) {

			_target4[_itargetNum[steps]] = combiB;
			_targetItem4[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiB);
			_itargetNum[steps]++;
		}
		else {

			_target4[_itargetNum[steps]] = combiB;
			_targetItem4[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiB);
			_itargetNum[steps]++;
		}



		_result4[ir[3]] = result;

		ir[3]++;
		if (_step < step)_step = step;
	}

	else if (step == 5) {

		if (typeA) {
			_target5[_itargetNum[steps]] = combiA;
			_targetItem5[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiA);
			_itargetNum[steps]++;
		}
		else {
			_target5[_itargetNum[steps]] = combiA;
			_targetItem5[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiA);
			_itargetNum[steps]++;
		}

		if (typeB) {
			_target5[_itargetNum[steps]] = combiB;
			_targetItem5[_itargetNum[steps]] = _xmlscene->getItemNameXML(combiB);
			_itargetNum[steps]++;
		}
		else {
			_target5[_itargetNum[steps]] = combiB;
			_targetItem5[_itargetNum[steps]] = _xmlscene->getBItemNameXML(combiB);
			_itargetNum[steps]++;
		}




		_result5[ir[4]] = result;

		ir[4]++;
		if (_step < step)_step = step;
	}


}


void CMix::doStep(float dt) {

	if (_bstate) {
		


		if (_btype) {
			if (_currentShown != _ifailureItem) {
				if (_count <= (_step+1) ) {

					if (_count == 1) {
						if (! (!strcmp(_mixItem[0], "B_water.png") || !strcmp(_mixItem[0], "B_glassrod.png"))) {
							_xmlscene->editItemState(_ioriginalItem, false, _Node);
							_xmlscene->editItemState(_mixItem[0], true, _Node, _ioriginalItem, _ifailureItem);
							_currentSteps = 1;
							_currentShown = _xmlscene->getItemNumXML(_mixItem[0],1);
						}
						else {
							_count = 0;
						}
							
					}
					else if (_count == 2) {
						if (_itargetNum[0] == 2) {
								if ((!strcmp(_mixItem[0], _targetItem1[0])) || (!strcmp(_mixItem[0], _targetItem1[1])) &&
									(!strcmp(_mixItem[1], _targetItem1[0])) || (!strcmp(_mixItem[1], _targetItem1[1]))) {
									if (((!strcmp(_mixItem[0], _targetItem1[0])) && (!strcmp(_mixItem[1], _targetItem1[0]))) ||
										((!strcmp(_mixItem[0], _targetItem1[1])) && (!strcmp(_mixItem[1], _targetItem1[1])))) {
										setFailure();
									}
									_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
									_xmlscene->editItemState(_result1[0], true, _Node);

									_currentShown = _result1[0];
									_currentSteps = 2;
								}
								else { 				//mix failure
									setFailure();
								}
							}

						else if (_itargetNum[0] == 4) {
								if (((!strcmp(_mixItem[0], _targetItem1[2])) || (!strcmp(_mixItem[0], _targetItem1[3]))) &&
									((!strcmp(_mixItem[1], _targetItem1[2])) || (!strcmp(_mixItem[1], _targetItem1[3])))) {
									if (((!strcmp(_mixItem[0], _targetItem1[2])) && (!strcmp(_mixItem[1], _targetItem1[2]))) ||
										((!strcmp(_mixItem[0], _targetItem1[3])) && (!strcmp(_mixItem[1], _targetItem1[3])))) {
										setFailure();
									}
									_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
									_xmlscene->editItemState(_result1[1], true, _Node);

									_currentShown = _result1[1];
									_currentSteps = 2;
								}

								else if (((!strcmp(_mixItem[0], _targetItem1[0])) || (!strcmp(_mixItem[0], _targetItem1[1]))) &&
									((!strcmp(_mixItem[1], _targetItem1[0])) || (!strcmp(_mixItem[1], _targetItem1[1]))
										)) {
									if (((!strcmp(_mixItem[0], _targetItem1[0])) && (!strcmp(_mixItem[1], _targetItem1[0]))) ||
										((!strcmp(_mixItem[0], _targetItem1[1])) && (!strcmp(_mixItem[1], _targetItem1[1])))) {
										setFailure();
									}
									_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
									_xmlscene->editItemState(_result1[0], true, _Node);

									_currentShown = _result1[0];
									_currentSteps = 2;
								}
								else { 				//mix failure
									setFailure();
								}
							}


					}



					else if (_count == 3) {

						auto current = _xmlscene->getItemNameXML(_currentShown);

						if (((!strcmp(_mixItem[2], _targetItem2[0])) || (!strcmp(_mixItem[2], _targetItem2[1]))) &&
							((!strcmp(current, _targetItem2[0])) || (!strcmp(current, _targetItem2[1])))) {
								if (((!strcmp(current, _targetItem2[0])) && (!strcmp(_mixItem[2], _targetItem2[0]))) ||
									((!strcmp(current, _targetItem2[1])) && (!strcmp(_mixItem[2], _targetItem2[1])))) {
									setFailure();
								}
								if (_result1[0] >= 0) _xmlscene->editItemState(_result1[0], false, _Node);
								if (_result1[1] >= 0) _xmlscene->editItemState(_result1[1], false, _Node);
								_xmlscene->editItemState(_result2[0], true, _Node);

								_currentShown = _result2[0];
								_currentSteps = 3;
							}

						else if (((!strcmp(_mixItem[2], _targetItem2[2])) || (!strcmp(_mixItem[2], _targetItem2[3]))) &&
								((!strcmp(current, _targetItem2[2])) || (!strcmp(current, _targetItem2[3])))) {
								if (((!strcmp(current, _targetItem2[2])) && (!strcmp(_mixItem[2], _targetItem2[2]))) ||
									((!strcmp(current, _targetItem2[3])) && (!strcmp(_mixItem[2], _targetItem2[3])))) {
									setFailure();
								}
								if (_result1[0] >= 0) _xmlscene->editItemState(_result1[0], false, _Node);
								if (_result1[1] >= 0) _xmlscene->editItemState(_result1[1], false, _Node);
								_xmlscene->editItemState(_result2[1], true, _Node);

								_currentShown = _result2[1];
								_currentSteps = 3;
							}

						else { 				//mix failure
							setFailure();
						}

						

					}



					else if (_count == 4) {

						auto current = _xmlscene->getItemNameXML(_currentShown);

						if (_itargetNum[2] > 2) {
							if (((!strcmp(_mixItem[3], _targetItem3[0])) || (!strcmp(_mixItem[3], _targetItem3[1]))) &&
								((!strcmp(current, _targetItem3[0])) || (!strcmp(current, _targetItem3[1])))) {
								if (((!strcmp(current, _targetItem3[0])) && (!strcmp(_mixItem[3], _targetItem3[0]))) ||
									((!strcmp(current, _targetItem3[1])) && (!strcmp(_mixItem[3], _targetItem3[1])))) {
									setFailure();
								}
								if (_result2[0] >= 0) _xmlscene->editItemState(_result2[0], false, _Node);
								if (_result2[1] >= 0) _xmlscene->editItemState(_result2[1], false, _Node);
								_xmlscene->editItemState(_result3[0], true, _Node);

								_currentShown = _result3[0];
								_currentSteps = 4;
							}


							else if (((!strcmp(_mixItem[3], _targetItem3[2])) || (!strcmp(_mixItem[3], _targetItem3[3]))) &&
								((!strcmp(current, _targetItem3[2])) || (!strcmp(current, _targetItem3[3])))) {
								if (((!strcmp(current, _targetItem3[2])) && (!strcmp(_mixItem[3], _targetItem3[2]))) ||
									((!strcmp(current, _targetItem3[3])) && (!strcmp(_mixItem[3], _targetItem3[3])))) {
									setFailure();
								}
								if (_result2[0] >= 0) _xmlscene->editItemState(_result2[0], false, _Node);
								if (_result2[1] >= 0) _xmlscene->editItemState(_result2[1], false, _Node);
								_xmlscene->editItemState(_result3[1], true, _Node);

								_currentShown = _result3[1];
								_currentSteps = 4;
							}
							else { 				//mix failure
								setFailure();
							}

						}

						else {
							if (((!strcmp(_mixItem[3], _targetItem3[0])) || (!strcmp(_mixItem[3], _targetItem3[1]))) &&
								((!strcmp(current, _targetItem3[0])) || (!strcmp(current, _targetItem3[1])))) {
								if (((!strcmp(current, _targetItem3[0])) && (!strcmp(_mixItem[3], _targetItem3[0]))) ||
									((!strcmp(current, _targetItem3[1])) && (!strcmp(_mixItem[3], _targetItem3[1])))) {
									setFailure();
								}
								if (_result2[0] >= 0) _xmlscene->editItemState(_result2[0], false, _Node);
								if (_result2[1] >= 0) _xmlscene->editItemState(_result2[1], false, _Node);
								_xmlscene->editItemState(_result3[0], true, _Node);

								_currentShown = _result3[0];
								_currentSteps = 4;
							}

							else { 				//mix failure
								setFailure();
							}
						}
							



	
					}


					else if (_count == 5) {

						auto current = _xmlscene->getItemNameXML(_currentShown);



						if (_itargetNum[3] > 2) {
							if (((!strcmp(_mixItem[4], _targetItem4[0])) || (!strcmp(_mixItem[4], _targetItem4[1]))) &&
								((!strcmp(current, _targetItem4[0])) || (!strcmp(current, _targetItem4[1])))) {
								if (((!strcmp(current, _targetItem4[0])) && (!strcmp(_mixItem[4], _targetItem4[0]))) ||
									((!strcmp(current, _targetItem4[1])) && (!strcmp(_mixItem[4], _targetItem4[1])))) {
									setFailure();
								}
								if (_result3[0] >= 0) _xmlscene->editItemState(_result3[0], false, _Node);
								if (_result3[1] >= 0) _xmlscene->editItemState(_result3[1], false, _Node);
								_xmlscene->editItemState(_result4[0], true, _Node);

								_currentShown = _result4[0];
								_currentSteps = 5;
							}

							else if (((!strcmp(_mixItem[4], _targetItem4[2])) || (!strcmp(_mixItem[4], _targetItem4[3]))) &&
								((!strcmp(current, _targetItem4[2])) || (!strcmp(current, _targetItem4[3])))) {
								if (((!strcmp(current, _targetItem4[2])) && (!strcmp(_mixItem[4], _targetItem4[2]))) ||
									((!strcmp(current, _targetItem4[3])) && (!strcmp(_mixItem[4], _targetItem4[3])))) {
									setFailure();
								}
								if (_result3[0] >= 0) _xmlscene->editItemState(_result3[0], false, _Node);
								if (_result3[1] >= 0) _xmlscene->editItemState(_result3[1], false, _Node);
								_xmlscene->editItemState(_result4[1], true, _Node);

								_currentShown = _result4[1];
								_currentSteps = 5;
							}
							else { 				//mix failure
								setFailure();
							}

						}

						else {
							if (((!strcmp(_mixItem[4], _targetItem4[0])) || (!strcmp(_mixItem[4], _targetItem4[1]))) &&
								((!strcmp(current, _targetItem4[0])) || (!strcmp(current, _targetItem4[1])))) {
								if (((!strcmp(current, _targetItem4[0])) && (!strcmp(_mixItem[4], _targetItem4[0]))) ||
									((!strcmp(current, _targetItem4[1])) && (!strcmp(_mixItem[4], _targetItem4[1])))) {
									setFailure();
								}
								if (_result3[0] >= 0) _xmlscene->editItemState(_result3[0], false, _Node);
								if (_result3[1] >= 0) _xmlscene->editItemState(_result3[1], false, _Node);
								_xmlscene->editItemState(_result4[0], true, _Node);

								_currentShown = _result4[0];
								_currentSteps = 5;
							}
							else { 				//mix failure
								setFailure();
							}

						}




					}


					else if (_count == 6) {
						
						auto current = _xmlscene->getItemNameXML(_currentShown);


						if (_itargetNum[4] > 2) {
							if (((!strcmp(_mixItem[5], _targetItem5[0])) || (!strcmp(_mixItem[5], _targetItem5[1]))) &&
								((!strcmp(current, _targetItem5[0])) || (!strcmp(current, _targetItem5[1])))) {
								if (((!strcmp(current, _targetItem5[0])) && (!strcmp(_mixItem[5], _targetItem5[0]))) ||
									((!strcmp(current, _targetItem5[1])) && (!strcmp(_mixItem[5], _targetItem5[1])))) {
									setFailure();
								}
								if (_result4[0] >= 0) _xmlscene->editItemState(_result4[0], false, _Node);
								if (_result4[1] >= 0) _xmlscene->editItemState(_result4[1], false, _Node);
								_xmlscene->editItemState(_result5[0], true, _Node);

								_currentShown = _result5[0];
								_currentSteps = 6;
							}

							else if (((!strcmp(_mixItem[5], _targetItem5[2])) || (!strcmp(_mixItem[5], _targetItem5[3]))) &&
								((!strcmp(current, _targetItem5[2])) || (!strcmp(current, _targetItem5[3])))) {
								if (((!strcmp(current, _targetItem5[2])) && (!strcmp(_mixItem[5], _targetItem5[2]))) ||
									((!strcmp(current, _targetItem5[3])) && (!strcmp(_mixItem[5], _targetItem5[3])))) {
									setFailure();
								}
								if (_result4[0] >= 0) _xmlscene->editItemState(_result4[0], false, _Node);
								if (_result4[1] >= 0) _xmlscene->editItemState(_result4[1], false, _Node);
								_xmlscene->editItemState(_result5[1], true, _Node);

								_currentShown = _result5[1];
								_currentSteps = 6;
							}
							else { 				//mix failure
								setFailure();
							}
						}

						else {
							if (((!strcmp(_mixItem[5], _targetItem5[0])) || (!strcmp(_mixItem[5], _targetItem5[1]))) &&
								((!strcmp(current, _targetItem5[0])) || (!strcmp(current, _targetItem5[1])))) {
								if (((!strcmp(current, _targetItem5[0])) && (!strcmp(_mixItem[5], _targetItem5[0]))) ||
									((!strcmp(current, _targetItem5[1])) && (!strcmp(_mixItem[5], _targetItem5[1])))) {
									setFailure();
								}
								if (_result4[0] >= 0) _xmlscene->editItemState(_result4[0], false, _Node);
								if (_result4[1] >= 0) _xmlscene->editItemState(_result4[1], false, _Node);
								_xmlscene->editItemState(_result5[0], true, _Node);

								_currentShown = _result5[0];
								_currentSteps = 6;
							}
							else { 				//mix failure
								setFailure();
							}
						}
							
					}

				}




				else {
					setFailure();
				}
			}


		}


		else {
			if (_count == 1) {
				_xmlscene->editItemState(_ioriginalItem, false, _Node);
				_xmlscene->editItemState(_mixItem[0], true, _Node, _ioriginalItem, _ifailureItem);
				_currentShown = _xmlscene->getItemNumXML(_mixItem[0],1);
			}
		}




		_bstate = !_bstate;
	}

}






//
////
////void CMix::SetCombinationB(int combiA, int combiB, int result) {
////	_targetItemB[0] = _xmlscene->getBItemNameXML(combiA);
////	_itargetNum++;
////
////	_targetItemB[1] = _xmlscene->getBItemNameXML(combiB);
////	_resultB = result;
////
////
////}
////
////
////void CMix::SetCombination(int combi, int result) {
////	_targetItem[_itargetNum] = _xmlscene->getBItemNameXML(combi);
////	_itargetNum++;
////
////
////	_result[ir] = result;
////	ir++;
////
////}
//
//void CMix::doStep(float dt) {
//	/*
//	if (_bstate) {
//		if (_count <= _itargetNum) {
//			if (_count == 1) {
//				_xmlscene->editItemState(_ioriginalItem, false, _Node);
//				_xmlscene->editItemState(_mixItem[0], true, _Node, _ioriginalItem, _ifailureItem);
//			}
//			else if (_count == 2) {
//
//				if ((!strcmp(_mixItem[0], _targetItem[0])) || (!strcmp(_mixItem[0], _targetItem[1])) &&
//					(!strcmp(_mixItem[1], _targetItem[0])) || (!strcmp(_mixItem[1], _targetItem[1]))) {
//					if ((!strcmp(_mixItem[0], _targetItem[0])) && (!strcmp(_mixItem[1], _targetItem[0])) ||
//						(!strcmp(_mixItem[0], _targetItem[1])) && (!strcmp(_mixItem[1], _targetItem[1]))){
//						_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//						_xmlscene->editItemState(_ifailureItem, true, _Node);
//					}
//					_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//					_xmlscene->editItemState(_result[0], true, _Node);
//				}
//				else if ((!strcmp(_mixItem[0], _targetItemB[0])) || (!strcmp(_mixItem[0], _targetItemB[1])) &&
//					(!strcmp(_mixItem[1], _targetItemB[0])) || (!strcmp(_mixItem[1], _targetItemB[1]))) {
//					if ((!strcmp(_mixItem[0], _targetItemB[0])) && (!strcmp(_mixItem[1], _targetItemB[0])) ||
//						(!strcmp(_mixItem[0], _targetItemB[1])) && (!strcmp(_mixItem[1], _targetItemB[1]))) {
//						_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//						_xmlscene->editItemState(_ifailureItem, true, _Node);
//					}
//					_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//					_xmlscene->editItemState(_resultB, true, _Node);
//					_mixB = true;
//				}
//				else { 				//mix failure
//					_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//					_xmlscene->editItemState(_ifailureItem, true, _Node);
//
//				}
//			}
//
//
//			else if (_count == 3) {
//
//
//				if ((!strcmp(_mixItem[2], _targetItem[2]))) {
//					_xmlscene->editItemState(_result[0], false, _Node);
//					_xmlscene->editItemState(_resultB, false, _Node);
//					_xmlscene->editItemState(_result[1], true, _Node);
//				}
//
//				else {
//					_xmlscene->editItemState(_resultB, false, _Node);
//					_xmlscene->editItemState(_result[0], false, _Node);
//					_xmlscene->editItemState(_ifailureItem, true, _Node);
//
//				}
//			}
//
//			else if (_count == 4) {
//			
//
//				if ((!strcmp(_mixItem[3], _targetItem[3]))) {
//					_xmlscene->editItemState(_result[1], false, _Node);
//					_xmlscene->editItemState(_result[2], true, _Node);
//				}
//
//
//				
//				else {
//					_xmlscene->editItemState(_result[1], false, _Node);
//					_xmlscene->editItemState(_ifailureItem, true, _Node);
//
//				}
//			}
//		}
//		else {
//			for (int i = 0; i<_count; i++) _xmlscene->editItemState(_mixItem[i], false, _Node, _ioriginalItem, _ifailureItem);
//			for (int i = 0; i<ir; i++) _xmlscene->editItemState(_result[i], false, _Node);
//			_xmlscene->editItemState(_ifailureItem, true, _Node);
//		}
//
//
//		_bstate = !_bstate;
//	}*/
//	
//	if (_bstate) {
//		if (_count <= _itargetNum) {
//			if (_count == 1) {
//				_xmlscene->editItemState(_ioriginalItem, false, _Node);
//				_xmlscene->editItemState(_mixItem[0], true, _Node, _ioriginalItem, _ifailureItem);
//			}
//			else if (_count == 2) {
//
//				if ((!strcmp(_mixItem[0], _targetItem[0])) || (!strcmp(_mixItem[0], _targetItem[1])) &&
//					(!strcmp(_mixItem[1], _targetItem[0])) || (!strcmp(_mixItem[1], _targetItem[1]))) {
//					if ((!strcmp(_mixItem[0], _targetItem[0])) && (!strcmp(_mixItem[1], _targetItem[0])) ||
//						(!strcmp(_mixItem[0], _targetItem[1])) && (!strcmp(_mixItem[1], _targetItem[1]))) {
//						_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//						_xmlscene->editItemState(_ifailureItem, true, _Node);
//					}
//					_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//					_xmlscene->editItemState(_result[0], true, _Node);
//				}
//				
//				else { 				//mix failure
//					_xmlscene->editItemState(_mixItem[0], false, _Node, _ioriginalItem, _ifailureItem);
//					_xmlscene->editItemState(_ifailureItem, true, _Node);
//
//				}
//			}
//
//
//			else if (_count == 3) {
//				if ((!strcmp(_mixItem[2], _targetItem[2]))) {
//					_xmlscene->editItemState(_result[0], false, _Node);
//					_xmlscene->editItemState(_resultB, false, _Node);
//					_xmlscene->editItemState(_result[1], true, _Node);
//				}
//
//				else {
//					_xmlscene->editItemState(_resultB, false, _Node);
//					_xmlscene->editItemState(_result[0], false, _Node);
//					_xmlscene->editItemState(_ifailureItem, true, _Node);
//
//				}
//			}
//
//			else if (_count == 4) {
//
//
//				if ((!strcmp(_mixItem[3], _targetItem[3]))) {
//					_xmlscene->editItemState(_result[1], false, _Node);
//					_xmlscene->editItemState(_result[2], true, _Node);
//				}
//
//
//
//				else {
//					_xmlscene->editItemState(_result[1], false, _Node);
//					_xmlscene->editItemState(_ifailureItem, true, _Node);
//
//				}
//			}
//		}
//		else {
//			for (int i = 0; i<_count; i++) _xmlscene->editItemState(_mixItem[i], false, _Node, _ioriginalItem, _ifailureItem);
//			for (int i = 0; i<ir; i++) _xmlscene->editItemState(_result[i], false, _Node);
//			_xmlscene->editItemState(_ifailureItem, true, _Node);
//		}
//
//
//		_bstate = !_bstate;
//	}
//
//}
//


void CMix::mixing(int item, bool type) {
	if(type) _mixItem[_count] = xmlBag::getInstance()->getItemName(item);
	else  _mixItem[_count] = _xmlscene->getItemNameXML(item);
	
	_count++;
	_bstate = !_bstate;
}


int CMix::getCurrentObj() {
	//auto current = _xmlscene->getItemNameXML(_currentShown);
	return _currentShown;
}

bool CMix::getCompleteStep() {
	if (_currentSteps == _step + 1) {
		return true;
	}
	else return false;
}


void CMix::setFailure() {

	_xmlscene->editRangeState(_ifailureItem, true, _Node, _ioriginalItem, _ifailureItem);

	_currentShown = _ifailureItem;




}

void CMix::setEmpty(int item, int start, int end) {

	_xmlscene->editRangeState(item, true, _Node, start, end);

	_currentShown = _ioriginalItem;




}



void CMix::Clear() {

	setEmpty(_ioriginalItem, _ioriginalItem, _ifailureItem);

	for (size_t i = 0; i < 7; i++) {
		_mixItem[i] = nullptr;
	}
	_bstate = false;
	_count = 0;

}


