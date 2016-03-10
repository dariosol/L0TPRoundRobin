/*
 MEPHeader.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: Jonas Kunze (kunze.jonas@gmail.com)
 */

#include "CustomMEP.h"
#include "CustomMEPFragment.h"

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <iostream>
#include <new>
#include <string>

//#include "exceptions/BrokenPacketReceivedError.h"
//#include "../exceptions/UnknownSourceIDFound.h"
//#include "../options/Options.h"

namespace na62 {
namespace l0 {


CustomMEP::CustomMEP(const char *data, const uint_fast16_t & dataLength) :
		rawData_(reinterpret_cast<const MEP_HDR*>(data)), checkSumsVarified_(
		false) {

	fragments_ = new CustomMEPFragment*[rawData_->eventCount];
	//if (getLength() != dataLength) {
	//	if (getLength() > dataLength) {
	//		throw BrokenPacketReceivedError(
	//				"Incomplete MEP! Received only "
	//						+ std::to_string(dataLength) + " of "
	//						+ std::to_string(getLength()) + " bytes");
	//	} else {
	//		throw BrokenPacketReceivedError(
	//				"Received MEP longer than 'mep length' field! Received "
	//						+ std::to_string(dataLength) + " instead of "
	//						+ std::to_string(getLength()) + " bytes");
	//	}
	//}

	initializeMEPFragments(data, dataLength);
}


void CustomMEP::initializeMEPFragments(const char * data, const uint_fast16_t& dataLength) {

	// The first subevent starts directly after the header -> offset is 12
	uint_fast16_t offset = sizeof(MEP_HDR);

	CustomMEPFragment* newMEPFragment;
	uint_fast32_t expectedEventNum = getFirstEventNum();

	for (uint_fast16_t i = 0; i < getNumberOfFragments(); i++) {
		/*
		 *  Throws exception if the event number LSB has an unexpected value
		 */
		newMEPFragment = new CustomMEPFragment(this,
				(MEPFragment_HDR*) (data + offset), expectedEventNum);

		expectedEventNum++;
		fragments_[i] = newMEPFragment;
		//if (newMEPFragment->getDataWithHeaderLength() + offset > dataLength) {
		//	throw BrokenPacketReceivedError(
		//			"Incomplete MEPFragment! Received only "
		//					+ std::to_string(dataLength) + " of "
		//					+ std::to_string(
		//							offset
		//									+ newMEPFragment->getDataWithHeaderLength())
		//					+ " bytes");
		//}
		offset += newMEPFragment->getDataWithHeaderLength();
	}

	// Check if too many bytes have been transmitted
	//if (offset < dataLength) {
	//	throw BrokenPacketReceivedError(
	//			"Sum of MEP events + MEP Header is smaller than expected: "
	//					+ std::to_string(offset) + " instead of "
	//					+ std::to_string(dataLength));
	//}
	eventCount_ = rawData_->eventCount;
}

CustomMEP::~CustomMEP() {
	//if (eventCount_ > 0) {
	//	/*
	//	 * TODO: Just for testing. Should be deleted later to boost performance!
	//	 */
	//	throw NA62Error("Deleting non-empty MEP!!!");
	//}
    int number = getNumberOfFragments();
   ////std::cout<<"elm: "<<number<<std::endl; 
	for (uint_fast16_t i = 0; i < number; i++) {
    //   std::cout<<"elm: "<<i<<std::endl;·
       delete fragments_[i];
    }
	delete[] fragments_;
}

} /* namespace l2 */
} /* namespace na62 */
