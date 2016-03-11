/*
 *  Created on: Mar 10, 2015
 *      Author: Marco Boretto marco.bore@gmail.com
 */

#include "CustomMEP.h"
#include "CustomMEPFragment.h"

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <iostream>
#include <new>
#include <string>

namespace na62 {
namespace l0 {


CustomMEP::CustomMEP(const char *data, const uint_fast16_t & dataLength) :
		rawData_(reinterpret_cast<const MEP_HDR*>(data)) {

	fragments_ = new CustomMEPFragment*[rawData_->eventCount];
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
		offset += newMEPFragment->getDataWithHeaderLength();
	}

	eventCount_ = rawData_->eventCount;
}

CustomMEP::~CustomMEP() {
    int number = getNumberOfFragments();
	for (uint_fast16_t i = 0; i < number; i++) {
       delete fragments_[i];
    }
	delete[] fragments_;
}

} /* namespace l2 */
} /* namespace na62 */
