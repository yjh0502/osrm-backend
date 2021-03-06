#include "catch.hpp"

#include <osmium/area/detail/node_ref_segment.hpp>

using osmium::area::detail::NodeRefSegment;

TEST_CASE("NodeRefSegmentClass") {

    SECTION("instantiation_with_default_parameters") {
        NodeRefSegment s;
        REQUIRE(s.first().ref() == 0);
        REQUIRE(s.first().location() == osmium::Location());
        REQUIRE(s.second().ref() == 0);
        REQUIRE(s.second().location() == osmium::Location());
    }

    SECTION("instantiation") {
        osmium::NodeRef nr1(1, { 1.2, 3.4 });
        osmium::NodeRef nr2(2, { 1.4, 3.1 });
        osmium::NodeRef nr3(3, { 1.2, 3.6 });
        osmium::NodeRef nr4(4, { 1.2, 3.7 });

        NodeRefSegment s1(nr1, nr2);
        REQUIRE(s1.first().ref() == 1);
        REQUIRE(s1.second().ref() == 2);

        NodeRefSegment s2(nr2, nr3);
        REQUIRE(s2.first().ref() == 3);
        REQUIRE(s2.second().ref() == 2);

        NodeRefSegment s3(nr3, nr4);
        REQUIRE(s3.first().ref() == 3);
        REQUIRE(s3.second().ref() == 4);
    }

    SECTION("intersection") {
        NodeRefSegment s1({ 1, {0.0, 0.0}}, { 2, {2.0, 2.0}});
        NodeRefSegment s2({ 3, {0.0, 2.0}}, { 4, {2.0, 0.0}});
        NodeRefSegment s3({ 5, {2.0, 0.0}}, { 6, {4.0, 2.0}});
        NodeRefSegment s4({ 7, {1.0, 0.0}}, { 8, {3.0, 2.0}});
        NodeRefSegment s5({ 9, {0.0, 4.0}}, {10, {4.0, 0.0}});
        NodeRefSegment s6({11, {0.0, 0.0}}, {12, {1.0, 1.0}});
        NodeRefSegment s7({13, {1.0, 1.0}}, {14, {3.0, 3.0}});

        REQUIRE(calculate_intersection(s1, s2) == osmium::Location(1.0, 1.0));
        REQUIRE(calculate_intersection(s2, s1) == osmium::Location(1.0, 1.0));

        REQUIRE(calculate_intersection(s1, s3) == osmium::Location());
        REQUIRE(calculate_intersection(s3, s1) == osmium::Location());

        REQUIRE(calculate_intersection(s2, s3) == osmium::Location());
        REQUIRE(calculate_intersection(s3, s2) == osmium::Location());

        REQUIRE(calculate_intersection(s1, s4) == osmium::Location());
        REQUIRE(calculate_intersection(s4, s1) == osmium::Location());

        REQUIRE(calculate_intersection(s1, s5) == osmium::Location(2.0, 2.0));
        REQUIRE(calculate_intersection(s5, s1) == osmium::Location(2.0, 2.0));

        REQUIRE(calculate_intersection(s1, s6) == osmium::Location(1.0, 1.0));
        REQUIRE(calculate_intersection(s6, s1) == osmium::Location(1.0, 1.0));

        REQUIRE(calculate_intersection(s1, s7) == osmium::Location(1.0, 1.0));
        REQUIRE(calculate_intersection(s7, s1) == osmium::Location(1.0, 1.0));

        REQUIRE(calculate_intersection(s6, s7) == osmium::Location());
        REQUIRE(calculate_intersection(s7, s6) == osmium::Location());
    }

    SECTION("intersection of collinear segments") {
        NodeRefSegment s1({ 1, {0.0, 0.0}}, { 2, {2.0, 0.0}}); // *---*
        NodeRefSegment s2({ 3, {2.0, 0.0}}, { 4, {4.0, 0.0}}); //     *---*
        NodeRefSegment s3({ 5, {0.0, 0.0}}, { 6, {1.0, 0.0}}); // *-*
        NodeRefSegment s4({ 7, {1.0, 0.0}}, { 8, {2.0, 0.0}}); //   *-*
        NodeRefSegment s5({ 9, {1.0, 0.0}}, {10, {3.0, 0.0}}); //   *---*
        NodeRefSegment s6({11, {0.0, 0.0}}, {12, {4.0, 0.0}}); // *-------*
        NodeRefSegment s7({13, {0.0, 0.0}}, {14, {5.0, 0.0}}); // *---------*
        NodeRefSegment s8({13, {1.0, 0.0}}, {14, {5.0, 0.0}}); //   *-------*
        NodeRefSegment s9({13, {3.0, 0.0}}, {14, {4.0, 0.0}}); //       *-*

        REQUIRE(calculate_intersection(s1, s1) == osmium::Location());

        REQUIRE(calculate_intersection(s1, s2) == osmium::Location());
        REQUIRE(calculate_intersection(s2, s1) == osmium::Location());

        REQUIRE(calculate_intersection(s1, s3) == osmium::Location(1.0, 0.0));
        REQUIRE(calculate_intersection(s3, s1) == osmium::Location(1.0, 0.0));

        REQUIRE(calculate_intersection(s1, s4) == osmium::Location(1.0, 0.0));
        REQUIRE(calculate_intersection(s4, s1) == osmium::Location(1.0, 0.0));

        REQUIRE(calculate_intersection(s1, s5) == osmium::Location(1.0, 0.0));
        REQUIRE(calculate_intersection(s5, s1) == osmium::Location(1.0, 0.0));

        REQUIRE(calculate_intersection(s1, s6) == osmium::Location(2.0, 0.0));
        REQUIRE(calculate_intersection(s6, s1) == osmium::Location(2.0, 0.0));

        REQUIRE(calculate_intersection(s1, s7) == osmium::Location(2.0, 0.0));
        REQUIRE(calculate_intersection(s7, s1) == osmium::Location(2.0, 0.0));

        REQUIRE(calculate_intersection(s1, s8) == osmium::Location(1.0, 0.0));
        REQUIRE(calculate_intersection(s8, s1) == osmium::Location(1.0, 0.0));

        REQUIRE(calculate_intersection(s1, s9) == osmium::Location());
        REQUIRE(calculate_intersection(s9, s1) == osmium::Location());

        REQUIRE(calculate_intersection(s5, s6) == osmium::Location(1.0, 0.0));
        REQUIRE(calculate_intersection(s6, s5) == osmium::Location(1.0, 0.0));

        REQUIRE(calculate_intersection(s7, s8) == osmium::Location(1.0, 0.0));
        REQUIRE(calculate_intersection(s8, s7) == osmium::Location(1.0, 0.0));
    }

    SECTION("intersection of very long segments") {
        NodeRefSegment s1({ 1, {90.0, 90.0}}, { 2, {-90.0, -90.0}});
        NodeRefSegment s2({ 1, {-90.0, 90.0}}, { 2, {90.0, -90.0}});
        REQUIRE(calculate_intersection(s1, s2) == osmium::Location(0.0, 0.0));

        NodeRefSegment s3({ 1, {-90.0, -90.0}}, { 2, {90.0, 90.0}});
        NodeRefSegment s4({ 1, {-90.0, 90.0}}, { 2, {90.0, -90.0}});
        REQUIRE(calculate_intersection(s3, s4) == osmium::Location(0.0, 0.0));

        NodeRefSegment s5({ 1, {-90.00000001, -90.0}}, { 2, {90.0, 90.0}});
        NodeRefSegment s6({ 1, {-90.0, 90.0}}, { 2, {90.0, -90.0}});
        REQUIRE(calculate_intersection(s5, s6) == osmium::Location(0.0, 0.0));
    }

    SECTION("ordering") {
        osmium::NodeRef node_ref1(1, { 1.0, 3.0 });
        osmium::NodeRef node_ref2(2, { 1.4, 2.9 });
        osmium::NodeRef node_ref3(3, { 1.2, 3.0 });
        osmium::NodeRef node_ref4(4, { 1.2, 3.3 });

        REQUIRE(node_ref1 < node_ref2);
        REQUIRE(node_ref2 < node_ref3);
        REQUIRE(node_ref1 < node_ref3);
        REQUIRE(node_ref1 >= node_ref1);

        REQUIRE( osmium::location_less()(node_ref1, node_ref2));
        REQUIRE(!osmium::location_less()(node_ref2, node_ref3));
        REQUIRE( osmium::location_less()(node_ref1, node_ref3));
        REQUIRE( osmium::location_less()(node_ref3, node_ref4));
        REQUIRE(!osmium::location_less()(node_ref1, node_ref1));
    }

}

TEST_CASE("Ordering of NodeRefSegments") {
    osmium::NodeRef nr0(0, { 0.0, 0.0 });
    osmium::NodeRef nr1(1, { 1.0, 0.0 });
    osmium::NodeRef nr2(2, { 0.0, 1.0 });
    osmium::NodeRef nr3(3, { 2.0, 0.0 });
    osmium::NodeRef nr4(4, { 0.0, 2.0 });
    osmium::NodeRef nr5(5, { 1.0, 1.0 });
    osmium::NodeRef nr6(6, { 2.0, 2.0 });
    osmium::NodeRef nr7(6, { 1.0, 2.0 });

    NodeRefSegment s1(nr0, nr1);
    NodeRefSegment s2(nr0, nr2);
    NodeRefSegment s3(nr0, nr3);
    NodeRefSegment s4(nr0, nr4);
    NodeRefSegment s5(nr0, nr5);
    NodeRefSegment s6(nr0, nr6);
    NodeRefSegment s7(nr0, nr7);

    // s1
    REQUIRE_FALSE(s1 < s1);
    REQUIRE(s2 < s1);
    REQUIRE(s1 < s3);
    REQUIRE(s4 < s1);
    REQUIRE(s5 < s1);
    REQUIRE(s6 < s1);
    REQUIRE(s7 < s1);

    // s2
    REQUIRE_FALSE(s2 < s2);
    REQUIRE(s2 < s3);
    REQUIRE(s2 < s4);
    REQUIRE(s2 < s5);
    REQUIRE(s2 < s6);
    REQUIRE(s2 < s7);

    // s3
    REQUIRE_FALSE(s3 < s3);
    REQUIRE(s4 < s3);
    REQUIRE(s5 < s3);
    REQUIRE(s6 < s3);
    REQUIRE(s7 < s3);

    // s4
    REQUIRE_FALSE(s4 < s4);
    REQUIRE(s4 < s5);
    REQUIRE(s4 < s6);
    REQUIRE(s4 < s7);

    // s5
    REQUIRE_FALSE(s5 < s5);
    REQUIRE(s5 < s6);
    REQUIRE(s7 < s5);

    // s6
    REQUIRE_FALSE(s6 < s6);
    REQUIRE(s7 < s6);

    // s7
    REQUIRE_FALSE(s7 < s7);
}

