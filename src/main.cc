#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "key.h"
#include "key_data.h"
#include "scad.h"
#include "transform.h"

using namespace scad;

constexpr bool kWriteTestKeys = false;
constexpr bool kIncludeDactylRef = true;
// Add the caps into the stl for testing.
constexpr bool kAddCaps = false;

enum class Direction { UP, DOWN, LEFT, RIGHT };

void AddShapes(std::vector<Shape>* shapes, std::vector<Shape> to_add) {
  for (Shape s : to_add) {
    shapes->push_back(s);
  }
}

Shape ConnectMainKeys(KeyData& d);

int main() {
  printf("generating..\n");
  TransformList key_origin;
  //  key_origin.RotateY(10);
  key_origin.Translate(-20, -40, 3);

  // This is where all of the logic to position the keys is done. Everything below is cosmetic
  // trying to build the case.
  KeyData d(key_origin);

  if (kWriteTestKeys) {
    std::vector<Shape> test_shapes;
    std::vector<Key*> test_keys;
    // &d.key_q, &d.key_w, &d.key_f, &d.key_3, &d.key_tab};
    test_keys = {
        //&d.key_th1, &d.key_th2, &d.key_th3, &d.key_th_top3, &d.key_th_top2,
        &d.key_q,
        &d.key_a,
        &d.key_w,
        &d.key_s,
        &d.key_d,
    };
    test_keys = {
        //&d.key_th1, &d.key_th2, &d.key_th3, &d.key_th_top3, &d.key_th_top2,
        &d.key_th1,
        &d.key_th2,
        &d.key_th3,
        //&d.key_th_top2,
        //&d.key_th_top3,
        &d.key_th_bottom2,
        //   &d.key_d,
    };
    test_keys = d.all_keys();
    for (Key* key : test_keys) {
      key->add_side_nub = true;
      key->add_top_nub = true;
      key->extra_z = 4;
      test_shapes.push_back(key->GetSwitch());
      if (kAddCaps) {
        test_shapes.push_back(key->GetCap().Color("red"));
      }
    }
    if (kIncludeDactylRef) {
      Shape dactyl_manuform = Import("dactyl_manuform.stl").Translate(9, 8, -3.5);
      Shape dactyl_cc = Import("dactyl_cc.stl");
      test_shapes.push_back(dactyl_manuform.Color("green", .6));
      //  test_shapes.push_back(dactyl_cc.Color("blue", .3));
    }
    UnionAll(test_shapes)
        // .Subtract(d.key_th_top2.GetInverseCap())
        .WriteToFile("test_keys.scad");
    return 0;
  }

  // Set all of the widths here. This must be done before calling any of GetTopLeft etc.

  // d.key_b.extra_width_bottom = 3;
  // d.key_th1.extra_width_top = 2;
  // d.key_th1.extra_width_left = 2;
  d.key_th_top3.extra_width_top = 4;
  d.key_th3.extra_width_right = 2;
  d.key_v.extra_width_bottom = 4;
  d.key_e.extra_width_top = 2;
  d.key_r.extra_width_top = 2;
  d.key_b.extra_z = 1;

  d.key_q.extra_width_left = 3;
  d.key_a.extra_width_left = 3;
  d.key_z.extra_width_left = 3;

  d.key_t.extra_width_right = 3;
  d.key_g.extra_width_right = 3;
  // d.key_b.extra_width_right = 3;

  std::vector<Shape> shapes;

  //
  // Thumb plate
  //

  shapes.push_back(ConnectMainKeys(d));
  shapes.push_back(Union(ConnectVertical(d.key_q, d.key_a),
                         ConnectVertical(d.key_a, d.key_z),
                         ConnectHorizontal(d.key_th2, d.key_th3),
                         ConnectHorizontal(d.key_th1, d.key_th2),
                         ConnectHorizontal(d.key_th_top2, d.key_th_top3),
                         // ConnectVertical(d.key_th_top2, d.key_th2),
                         // ConnectVertical(d.key_th_top3, d.key_th3),
                         ConnectDiagonal(d.key_th_top2, d.key_th_top3, d.key_th3, d.key_th2)));

  shapes.push_back(TriFan(d.key_x.GetBottomRight(),
                          {
                              d.key_c.GetBottomLeft(),
                              d.key_left_arrow.GetTopLeft(),
                              d.key_left_arrow.GetBottomLeft(),
                              d.key_z.GetBottomRight(),
                              d.key_x.GetBottomLeft(),
                          }));
  shapes.push_back(TriFan(d.key_left_arrow.GetBottomRight(),
                          {
                              d.key_th1.GetTopLeft(),
                              d.key_th1.GetBottomLeft(),
                              d.key_left_arrow.GetBottomLeft(),
                          }));

  shapes.push_back(TriFan(d.key_v.GetBottomLeft(),
                          {
                              d.key_v.GetBottomRight(),
                              d.key_th1.GetTopLeft(),
                          }));
  shapes.push_back(TriFan(d.key_v.GetBottomRight(),
                          {
                              d.key_b.GetBottomLeft(),
                              d.key_th1.GetTopRight(),
                              d.key_th1.GetTopLeft(),
                          }));
  shapes.push_back(TriFan(d.key_v.GetBottomLeft(),
                          {
                              d.key_c.GetBottomRight(),
                              d.key_left_arrow.GetTopRight(),
                              d.key_left_arrow.GetBottomRight(),
                              d.key_th1.GetTopLeft(),
                          }));

  shapes.push_back(TriFan(d.key_th_top2.GetBottomLeft(),
                          {
                              d.key_th2.GetTopLeft(),
                              d.key_th2.GetTopRight(),
                              d.key_th_top2.GetBottomRight(),
                          }));
  shapes.push_back(TriFan(d.key_th_top3.GetBottomLeft(),
                          {
                              d.key_th3.GetTopLeft(),
                              d.key_th3.GetTopRight(),
                              d.key_th_top3.GetBottomRight(),
                          }));

  shapes.push_back(TriFan(d.key_th3.GetTopRight(),
                          {
                              d.key_th_top3.GetBottomRight(),
                              d.key_th3.GetBottomRight(),
                          }));
  shapes.push_back(TriFan(d.key_th_top2.GetTopRight(),
                          {
                              d.key_th_top3.GetTopLeft(),
                              d.key_th_top3.GetTopRight(),
                          }));
  shapes.push_back(TriFan(d.key_th_top2.GetTopRight(),
                          {
                              d.key_th_top2.GetTopLeft(),
                              d.key_b.GetTopRight(),
                              d.key_g.GetBottomRight(),
                          }));
  shapes.push_back(TriFan(d.key_b.GetBottomRight(),
                          {
                              d.key_th_top2.GetTopLeft(),
                              d.key_th_top2.GetBottomLeft(),
                          }));
  shapes.push_back(TriFan(d.key_th2.GetTopLeft(),
                          {
                              d.key_th_top2.GetBottomLeft(),
                              d.key_b.GetBottomRight(),
                              d.key_b.GetBottomLeft(),
                              d.key_th1.GetTopRight(),
                          }));

  shapes.push_back(TriMesh({
      d.key_q.GetTopLeft(),
      d.key_q.GetTopRight(),
      d.key_w.GetTopLeft(),
  }));

  shapes.push_back(TriMesh({
      d.key_w.GetTopLeft(),
      d.key_w.GetTopRight(),
      d.key_e.GetTopLeft(),
  }));

  shapes.push_back(TriMesh({
      d.key_e.GetTopRight(),
      d.key_r.GetTopLeft(),
      d.key_r.GetTopRight(),
  }));

  shapes.push_back(TriMesh({
      d.key_r.GetTopRight(),
      d.key_t.GetTopLeft(),
      d.key_t.GetTopRight(),
  }));

  //
  // Make the wall
  //
  {
    struct WallPoint {
      WallPoint(TransformList transforms,
                Direction out_direction,
                float extra_distance = 0,
                float extra_width = 0)
          : transforms(transforms),
            out_direction(out_direction),
            extra_distance(extra_distance),
            extra_width(extra_width) {
      }
      TransformList transforms;
      Direction out_direction;
      float extra_distance;
      float extra_width;
    };

    Direction up = Direction::UP;
    Direction down = Direction::DOWN;
    Direction left = Direction::LEFT;
    Direction right = Direction::RIGHT;

    std::vector<WallPoint> wall_points = {

        //{d.key_w.GetTopLeft(), up},
        //        {d.key_w.GetTopRight(), up},

         {d.key_2.GetTopLeft(), up},

        {d.key_3.GetTopLeft(), up},
        {d.key_3.GetTopRight(), up},

        {d.key_4.GetTopRight(), up},

        //       {d.key_t.GetTopLeft(), up},
        {d.key_t.GetTopRight(), up, 1},
        {d.key_t.GetTopRight(), right},
        {d.key_t.GetBottomRight(), right},

        {d.key_g.GetTopRight(), right},
        {d.key_g.GetBottomRight(), right},

        //{d.key_b.GetTopRight(), right},

        {d.key_th_top2.GetTopRight().RotateFront(0, 0, -20), up, 0, 0},

        {d.key_th_top3.GetTopRight(), up},
        {d.key_th_top3.GetTopRight(), right},
        {d.key_th_top3.GetBottomRight(), right},

        {d.key_th3.GetBottomRight(), right},

        {d.key_th_bottom2.GetBottomRight(), right},
        {d.key_th_bottom2.GetBottomRight(), down},
        {d.key_th_bottom2.GetBottomLeft(), down},

        {d.key_th1.GetBottomLeft(), down},

        {d.key_left_arrow.GetBottomLeft().RotateFront(0, 0, -20), down, 0, 1},

        {d.key_z.GetBottomRight(), down},
        {d.key_z.GetBottomLeft(), down},
        {d.key_z.GetBottomLeft(), left},

        {d.key_a.GetBottomLeft(), left},
        {d.key_a.GetTopLeft(), left},

        {d.key_q.GetBottomLeft(), left},
        {d.key_q.GetTopLeft(), left},
        {d.key_q.GetTopLeft(), up, 1, 0},
        //{d.key_q.GetTopRight(), up},

        //{d.key_1.GetTopRight().RotateFront(0, 0, 30), up, 0, 1},

    };

    std::vector<std::vector<Shape>> wall_slices;
    for (WallPoint point : wall_points) {
      Shape s1 = point.transforms.Apply(GetPostConnector());

      TransformList t = point.transforms;
      glm::vec3 out_dir;
      float distance = 4.8 + point.extra_distance;
      switch (point.out_direction) {
        case Direction::UP:
          t.AppendFront(TransformList().Translate(0, distance, 0).RotateX(-20));
          break;
        case Direction::DOWN:
          t.AppendFront(TransformList().Translate(0, -1 * distance, 0).RotateX(20));
          break;
        case Direction::LEFT:
          t.AppendFront(TransformList().Translate(-1 * distance, 0, 0).RotateY(-20));
          break;
        case Direction::RIGHT:
          t.AppendFront(TransformList().Translate(distance, 0, 0).RotateY(20));
          break;
      }

      // Make sure the section extruded to the bottom is thick enough. With certain angles the
      // projection is very small if you just use the post connector from the transform. Compute
      // an explicit shape.
      const glm::vec3 post_offset(0, 0, -4);
      const glm::vec3 p = point.transforms.Apply(post_offset);
      const glm::vec3 p2 = t.Apply(post_offset);

      glm::vec3 out_v = p2 - p;
      out_v.z = 0;
      const glm::vec3 in_v = -1.f * glm::normalize(out_v);

      float width = 3.3 + point.extra_width;
      Shape s2 = Hull(Cube(.1).Translate(p2), Cube(.1).Translate(p2 + (width * in_v)));

      std::vector<Shape> slice;
      slice.push_back(Hull(s1, s2));
      slice.push_back(Hull(s2, s2.Projection().LinearExtrude(.1).TranslateZ(.05)));

      wall_slices.push_back(slice);
    }

    for (size_t i = 0; i < wall_slices.size(); ++i) {
      auto& slice = wall_slices[i];
      auto& next_slice = wall_slices[(i + 1) % wall_slices.size()];
      for (size_t j = 0; j < slice.size(); ++j) {
        shapes.push_back(Hull(slice[j], next_slice[j]));
        // Uncomment for testing. Much faster and easier to visualize.
        // shapes.push_back(slice[j]);
      }
    }
  }

  for (Key* key : d.all_keys()) {
    shapes.push_back(key->GetSwitch());
    if (kAddCaps) {
      shapes.push_back(key->GetCap().Color("red"));
    }
  }

  // Add all the screw inserts.
  std::vector<Shape> screw_holes;
  {
    double screw_height = 5;
    double screw_radius = 4.4 / 2.0;
    Shape screw_hole = Cylinder(screw_height + .1, screw_radius, 30).TranslateZ(screw_height / 2);
    Shape screw_insert =
        Cylinder(screw_height, screw_radius + 1.65, 30).TranslateZ(screw_height / 2);

    glm::vec3 screw_left_bottom = d.key_z.GetBottomLeft().Apply(kOrigin);
    screw_left_bottom.z = 0;
    screw_left_bottom.x += 3.2;

    glm::vec3 screw_left_top = d.key_q.GetTopLeft().Apply(kOrigin);
    screw_left_top.z = 0;
    screw_left_top.x += 2.8;
    screw_left_top.y += -.5;

    glm::vec3 screw_right_top = d.key_t.GetTopRight().Apply(kOrigin);
    screw_right_top.z = 0;
    screw_right_top.x -= 0;
    screw_right_top.y += -.5;

    glm::vec3 screw_right_bottom = d.key_th_bottom2.GetBottomRight().Apply(kOrigin);
    screw_right_bottom.z = 0;
    screw_right_bottom.y += 4.5;
    screw_right_bottom.x += -1;

    glm::vec3 screw_right_mid = d.key_th_top3.GetTopRight().Apply(kOrigin);
    screw_right_mid.z = 0;
    screw_right_mid.y += 0;
    screw_right_mid.x += -3.5;

    shapes.push_back(Union(screw_insert.Translate(screw_left_top),
                           screw_insert.Translate(screw_right_top),
                           screw_insert.Translate(screw_right_mid),
                           screw_insert.Translate(screw_right_bottom),
                           screw_insert.Translate(screw_left_bottom)));

    screw_holes = {
        screw_hole.Translate(screw_left_top),
        screw_hole.Translate(screw_right_top),
        screw_hole.Translate(screw_right_mid),
        screw_hole.Translate(screw_right_bottom),
        screw_hole.Translate(screw_left_bottom),
    };
  }

  std::vector<Shape> negative_shapes;

  Key b_cut = d.key_b;
  b_cut.Configure([&](Key& k) {
    k.AddTransform();
    k.t().z = -4;
    k.add_side_nub = false;
    // k.disable_switch_z_offset = true;
  });
  negative_shapes.push_back(b_cut.GetInverseSwitch());
  AddShapes(&negative_shapes, screw_holes);
  // Cut off the parts sticking up into the thumb plate.

  // Cut out holes for cords. Inserts can be printed to fit in.
  Shape trrs_hole = Cylinder(20, 5, 30).RotateX(90);

  glm::vec3 trrs_hole_location = d.key_r.GetTopRight().Apply(kOrigin);
  trrs_hole_location.z = 11;
  trrs_hole_location.x -= 5;
  negative_shapes.push_back(trrs_hole.Translate(trrs_hole_location));

  {
    glm::vec3 usb_location = d.key_e.GetTopLeft().Apply({10, 0, 0});
    usb_location.z = 6;
    usb_location.y += 4;
    Shape c = Cylinder(8, 2.5, 30).RotateX(90).Translate(usb_location);
    negative_shapes.push_back(Hull(c, c.Projection().LinearExtrude(.1)));
  }

  Shape result = UnionAll(shapes);
  // Subtracting is expensive to preview and is best to disable while testing.
  result = result.Subtract(UnionAll(negative_shapes));
  result.WriteToFile("left.scad");
  result.MirrorX().WriteToFile("right.scad");

  // Bottom plate
  {
    std::vector<Shape> bottom_plate_shapes = {result};
    for (Key* key : d.all_keys()) {
      bottom_plate_shapes.push_back(Hull(key->GetSwitch()));
    }

    Shape bottom_plate = UnionAll(bottom_plate_shapes)
                             .Projection()
                             .LinearExtrude(1.5)
                             .Subtract(UnionAll(screw_holes));
    bottom_plate.WriteToFile("bottom_left.scad");
    bottom_plate.MirrorX().WriteToFile("bottom_right.scad");
  }

  return 0;
}

Shape ConnectMainKeys(KeyData& d) {
  std::vector<Shape> shapes;
  for (int r = 0; r < d.grid.num_rows(); ++r) {
    for (int c = 2; c < d.grid.num_columns(); ++c) {
      Key* key = d.grid.get_key(r, c);
      if (!key) {
        // No key at this location.
        continue;
      }
      Key* left = d.grid.get_key(r, c - 1);
      Key* top_left = d.grid.get_key(r - 1, c - 1);
      Key* top = d.grid.get_key(r - 1, c);

      if (left) {
        shapes.push_back(ConnectHorizontal(*left, *key));
      }
      if (top) {
        shapes.push_back(ConnectVertical(*top, *key));
        if (left && top_left) {
          shapes.push_back(ConnectDiagonal(*top_left, *top, *key, *left));
        }
      }
    }
  }
  return UnionAll(shapes);
}
