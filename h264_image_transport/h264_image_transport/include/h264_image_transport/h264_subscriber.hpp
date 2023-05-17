// Copyright (c) 2021, Clyde McQueen.
// All rights reserved.
//
// Software License Agreement (BSD License 2.0)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//  * Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef H264_IMAGE_TRANSPORT__H264_SUBSCRIBER_HPP_
#define H264_IMAGE_TRANSPORT__H264_SUBSCRIBER_HPP_

#include <string>

#include "image_transport/subscriber_plugin.hpp"
#include "h264_msgs/msg/packet.hpp"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}

namespace h264_image_transport
{

class H264Subscriber : public image_transport::SubscriberPlugin
{
private:
  rclcpp::Logger logger_;
  rclcpp::Subscription<h264_msgs::msg::Packet>::SharedPtr sub_;

  int64_t seq_;
  int consecutive_receive_failures_;
  AVCodec * p_codec_;
  AVCodecContext * p_codec_context_;
  AVFrame * p_frame_;
  AVPacket * p_packet_;
  SwsContext * p_sws_context_;

  void internalCallback(
    const h264_msgs::msg::Packet::ConstSharedPtr & message,
    const Callback & user_cb);

protected:
  void subscribeImpl(
    rclcpp::Node * node,
    const std::string &,
    const Callback &,
    rmw_qos_profile_t) override
  {
    RCLCPP_FATAL(node->get_logger(), "not used in Humble+");
  }

  void subscribeImpl(
    rclcpp::Node * node,
    const std::string & base_topic,
    const Callback & callback,
    rmw_qos_profile_t custom_qos,
    rclcpp::SubscriptionOptions options) override;

public:
  H264Subscriber();

  ~H264Subscriber() override;

  std::string getTransportName() const override
  {
    return "h264";
  }

  std::string getTopic() const override
  {
    if (sub_) {
      return sub_->get_topic_name();
    }
    return {};
  }

  size_t getNumPublishers() const override
  {
    if (sub_) {
      return sub_->get_publisher_count();
    }
    return 0;
  }

  void shutdown() override
  {
    sub_.reset();
  }
};

}  // namespace h264_image_transport

#endif  // H264_IMAGE_TRANSPORT__H264_SUBSCRIBER_HPP_
