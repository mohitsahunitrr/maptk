/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <test_common.h>

#include <maptk/ocv/feature_set.h>



#define TEST_ARGS ()

DECLARE_TEST_MAP();

int
main(int argc, char* argv[])
{
  CHECK_ARGS(1);

  testname_t const testname = argv[1];

  RUN_TEST(testname);
}


IMPLEMENT_TEST(default_set)
{
  using namespace maptk;
  ocv::feature_set fs;
  if (fs.size() != 0)
  {
    TEST_ERROR("Default features_set is not empty");
  }
  if (!fs.features().empty())
  {
    TEST_ERROR("Default features_set produces non-empty features");
  }
}


// It seems operator== on cv::Keypoint is not defined in OpenCV
static bool keypoints_equal(const cv::KeyPoint& kp1, const cv::KeyPoint& kp2)
{
  return kp1.angle == kp2.angle &&
         kp1.class_id == kp2.class_id &&
         kp1.octave == kp2.octave &&
         kp1.pt == kp2.pt &&
         kp1.response == kp2.response &&
         kp1.size == kp2.size;
}


IMPLEMENT_TEST(populated_set)
{
  using namespace maptk;
  const unsigned num_feat = 100;
  std::vector<cv::KeyPoint> kpts;
  for (unsigned i=0; i<num_feat; ++i)
  {
    cv::KeyPoint kp(i/2.0f, i/3.0f, i/10.0, (i*3.14159f)/num_feat, 100.0f/i);
    kpts.push_back(kp);
  }
  ocv::feature_set fs(kpts);
  if (fs.size() != num_feat)
  {
    TEST_ERROR("feature_set is not the expected size");
  }
  std::vector<cv::KeyPoint> kpts2 = fs.ocv_keypoints();
  if (!std::equal(kpts.begin(), kpts.end(), kpts2.begin(), keypoints_equal))
  {
    TEST_ERROR("feature_set does not contain original cv::KeyPoints");
  }
  std::vector<feature_sptr> feats = fs.features();
  if (feats.size() != fs.size())
  {
    TEST_ERROR("feature_set does not produce expected number of features");
  }
  for( unsigned i=0; i<num_feat; ++i)
  {
    if (feats[i]->data_type() != typeid(float))
    {
      TEST_ERROR("some features do not have the expected type (float)");
      break;
    }
  }

  simple_feature_set simp_fs(feats);
  kpts2 = ocv::features_to_ocv_keypoints(simp_fs);
  if (!std::equal(kpts.begin(), kpts.end(), kpts2.begin(), keypoints_equal))
  {
    TEST_ERROR("conversion to and from MAPTK features does not preserve cv::KeyPoints");
  }
}