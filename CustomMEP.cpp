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


CustomMEP::CustomMEP(int max_mep_number){ 

        rawData_ = nullptr;
	fragments_ = new CustomMEPFragment*[max_mep_number];

	// The first subevent starts directly after the header -> offset is 12
	uint_fast16_t offset = sizeof(MEP_HDR);
	CustomMEPFragment* newMEPFragment;
	for (uint_fast16_t i = 0; i < max_mep_number; i++) {
		/*
		 *  Throws exception if the event number LSB has an unexpected value
		 */
		newMEPFragment = new CustomMEPFragment(this);

		fragments_[i] = newMEPFragment;
	}

	//initializeMEPFragments(data, dataLength);
}


void CustomMEP::initializeMEPFragments(const char * data, const uint_fast16_t& dataLength) {

        is_last_burst_packet_ = false;
	rawData_ = reinterpret_cast<const MEP_HDR*>(data);
		
	// The first subevent starts directly after the header -> offset is 12
	uint_fast16_t offset = sizeof(MEP_HDR);

	uint_fast32_t expectedEventNum = getFirstEventNum();

	for (uint_fast16_t i = 0; i < getNumberOfFragments(); i++) {
                fragments_[i]->initilizeCustomMEPFragment((MEPFragment_HDR*) (data + offset), expectedEventNum);
                if (fragments_[i]->isLastEventOfBurst()) {

                    is_last_burst_packet_ = true;
                    //std::cout<<"I'm the last fragment of the burst!"<<std::endl;
                }
		expectedEventNum++;
		offset += fragments_[i]->getDataWithHeaderLength();
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
