/*!
 * \file
 * \brief
 */

#ifndef SETHSV_PROCESSOR_HPP_
#define SETHSV_PROCESSOR_HPP_

#include <opencv2/core/core.hpp>
#include <string>

#include <boost/shared_ptr.hpp>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Types/Mrrocpp_Proxy/BReading.hpp"

#include "xdr/xdr_iarchive.hpp"
#include "xdr/xdr_oarchive.hpp"

using namespace cv;

namespace Processors {
namespace SetHSV {

/*!
 * \brief SetHSV properties
 */
struct SetHSV_Props: public Base::Props
{

	cv::Mat blue_params;
	cv::Mat red_params;
	cv::Mat green_params;
	cv::Mat yellow_params;
 
	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		cout << "Loading properties..." << endl;

		boost::numeric::ublas::matrix <double> blueMatrixUblas = str2mat(pt.get <std::string> ("blue"), 3, 2);
		blue_params = cv::Mat(3, 2, CV_32F);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 2; ++j) {//#include "Types/Mrrocpp_Proxy/BReading.hpp"

				blue_params.at <float> (i, j) = blueMatrixUblas(i, j);
			}
		}

		boost::numeric::ublas::matrix <double> redMatrixUblas = str2mat(pt.get <std::string> ("red"), 3, 2);
		red_params = cv::Mat(3, 2, CV_32F);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 2; ++j) {
				red_params.at <float> (i, j) = redMatrixUblas(i, j);
			}
		}

		boost::numeric::ublas::matrix <double> greenMatrixUblas = str2mat(pt.get <std::string> ("green"), 3, 2);
		green_params = cv::Mat(3, 2, CV_32F);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 2; ++j) {
				green_params.at <float> (i, j) = greenMatrixUblas(i, j);
			}
		}

		boost::numeric::ublas::matrix <double> yellowMatrixUblas = str2mat(pt.get <std::string> ("yellow"), 3, 2);
		yellow_params = cv::Mat(3, 2, CV_32F);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 2; ++j) {
				yellow_params.at <float> (i, j) = yellowMatrixUblas(i, j);
			}
		}
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
	}

};

/*!
 * \class SetHSV_Processor
 * \brief SetHSV processor class.
 */
class SetHSV_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	SetHSV_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~SetHSV_Processor();

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

	/*!
	 * OnNewImage component
	 */
	void onNewImage();

	/*!
	 * OnRpcCall component
	 */
	void onRpcCall();

	Base::EventHandler <SetHSV_Processor> h_onNewImage;
	Base::EventHandler <SetHSV_Processor> h_onRpcCall;

	Base::Event * newImage;
	Base::Event * rpcResult;

	Base::DataStreamIn <cv::Mat, Base::DataStreamBuffer::Newest> in_img;

	Base::DataStreamIn <xdr_iarchive <>, Base::DataStreamBuffer::Newest> in_rpc;
	Base::DataStreamOut <Types::Mrrocpp_Proxy::BReading> out_rpc;

	Base::DataStreamOut <cv::Mat> out_hue;
	Base::DataStreamOut <cv::Mat> out_saturation;
	Base::DataStreamOut <cv::Mat> out_value;
	Base::DataStreamOut <cv::Mat> out_threshold;

	/// Properties
	SetHSV_Props props;

private:

	xdr_iarchive <> xdr_color;

	cv::Mat hue_img;
	cv::Mat saturation_img;
	cv::Mat value_img;
	cv::Mat threshold_img;

	cv::Mat params;
	uint32_t color;
};

}//: namespace SetHSV
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_COMPONENT("SetHSVsimple", Processors::SetHSV::SetHSV_Processor)

#endif /* SETHSV_PROCESSOR_HPP_ */
