/*
 *  Created on: Mar 10, 2015
 *      Author: Marco Boretto marco.bore@gmail.com
 */

#include <string>
#include <iostream>
#include "CustomMEP.h"  // forward declaration
#include "CustomMEPFragment.h"

using namespace na62;
namespace na62 {
namespace l0 {

CustomMEPFragment::CustomMEPFragment(CustomMEP* mep) :
		mep_(mep) {
}

void CustomMEPFragment::initilizeCustomMEPFragment(const MEPFragment_HDR *data,
		uint_fast32_t& expectedEventNum) {
         rawData = data;
         eventNumber_ = expectedEventNum;

	/*
	 * Cite from NA62-11-02:
	 * Event number LSB: the least significant 16 bits of the event number, as defined inside the
	 * transmitter by the number of L0 triggers received since the start of the burst; the most
	 * significant 8 bits are obtained from the MEP header. For the first event in the MEP, this field
	 * will match the lower 16 bits of the first word in the MEP header; since all sub-systems in global
	 * mode must respond to all L0 triggers, for each following event in the MEP, this number should
	 * increase by one, possibly wrapping around to zero (in which case the upper 8 bits of the event
	 * number are those in the MEP header incremented by one).
	 */
	if (rawData->eventNumberLSB_ != (expectedEventNum & 0x000000FF)
        ){
			//			|| ((!(expectedEventNum % 3125)) && (expectedEventNum > 50))) {
		std::cout<<"++++++++++++++MEP SourceID " << (uint)(mep_->getSourceID()) << std::endl;
		std::cout<<"++++++++++++++MEP Length " << (uint)(mep_->getLength()) << std::endl;
		std::cout<<"++++++++++++++MEP FirstEvtNum " << (uint)(mep_->getFirstEventNum()) << std::endl;
		std::cout<<"++++++++++++++MEP mep_Factor " << (uint)(mep_->getNumberOfFragments()) << std::endl;
		std::cout<<"++++++++++++++ExpEvtNum " << (uint)expectedEventNum << std::endl;

		int* d=(int*)data;
		for (int ilength = 0; ilength < sizeof(MEPFragment_HDR)/4;
				ilength++) {
			std::cout<< "++++++++++++MEP fragment (sizeof(MEPFragment_HDR)) " << (int)(sizeof(MEPFragment_HDR))
			<< " index " << (int) ilength
			<< " data " << std::hex << *d << std::dec << std::endl;
			d++;
		}
	}
}

CustomMEPFragment::~CustomMEPFragment() {
}

/*
 * The sourceID in the header of this MEP event
 */
uint_fast8_t CustomMEPFragment::getSourceID() const {
	return mep_->getSourceID();
}

/*
 * The sourceSubID in the header of this MEP event
 */
uint_fast8_t CustomMEPFragment::getSourceSubID() const {
	return mep_->getSourceSubID();
}

} /* namespace l0 */
} /* namespace na62 */
