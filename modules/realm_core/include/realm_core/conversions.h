

#ifndef PROJECT_GIS_CONVERSIONS_H
#define PROJECT_GIS_CONVERSIONS_H

#include <opencv2/opencv.hpp>
#include <gdal/ogr_spatialref.h>

#include <realm_core/utm32.h>
#include <realm_core/wgs84.h>

namespace realm
{
namespace gis
{

/*!
 * @brief Converter for a pose (position and heading only) in WGS84 to UTM coordinate frame
 * @param wgs Pose in WGS84 coordinate frame
 * @return Pose in UTM coordinate frame
 */
UTMPose convertToUTM(const WGSPose &wgs);

/*!
 * @brief Converter for a pose (position and heading only) in UTM to WGS84 coordinate frame
 * @param wgs Pose in UTM coordinate frame
 * @return Pose in WGS84 coordinate frame
 */
WGSPose convertToWGS84(const UTMPose &utm);

/*!
 * @brief GDAL 3 changes axis order: https://github.com/OSGeo/gdal/blob/master/gdal/MIGRATION_GUIDE.TXT
 * @param oSRS Spatial reference to be patched depending on the installed GDAL version
 */
void initAxisMappingStrategy(OGRSpatialReference *oSRS);

}

namespace pose
{
    /*! TODO: Einbaurichtung implementieren?
 * @brief Function to compute a 3x3 rotation matrix based on heading data. It is assumed, that the camera is pointing
 * downwards and the heading roughly aligns with the camera's yaw axis.
 * @param heading Magnetic heading of the camera / UAV
 * @return 3x3 rotation matrix
 */
    cv::Mat computeOrientationFromHeading(double heading);

/*! TODO: Make this more general to get full yaw/pitch/roll
 * @brief Get yaw from a 3x3 rotation matrix It is assumed, that the camera is pointing
 * downwards and the heading roughly aligns with the camera's yaw axis.
 * @param orientation The orientation matrix to use
 * @return The yaw in degrees
 */
    double getHeadingFromOrientation(const cv::Mat& orientation);

}
}

#endif //PROJECT_GIS_CONVERSIONS_H
