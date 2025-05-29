#ifndef ALGUI_UILAYOUTNODE_HPP
#define ALGUI_UILAYOUTNODE_HPP


#include <limits.h>
#include "UIPositionedNode.hpp"
#include "Layout.hpp"


namespace algui {


    class UILayoutNode : public UIPositionedNode {
    public:
        virtual ~UILayoutNode();

        /**
         * Sets the visible state of the object.
         * If the object changes visibility, then the layout of the parent is invalidated.
         * @param v the new visible state.
         */
        void setVisible(bool v) override;

        /**
         * Interface for checking if the underlying node is a layout node.
         * @return true.
         */
        bool isLayoutNode() const override {
            return true;
        }

        /**
         * Checks if the node's geometry, position and layout are valid.
         * @return true if the geometry and the position and the layout are valid, false otherwise.
         */
        bool isGeometryValid() const override;

        /**
         * Sets the geometry and the position and the layout to invalid state.
         */
        void invalidateGeometry() override;

        /**
         * Returns the closest ancestor node that is a layout node.
         * @return the closest ancestor node that is a layout node.
         */
        UILayoutNode* getParent() const;

        /**
         * Returns the closest previous sibling node that is a layout node.
         * @return the closest previous sibling node that is a layout node.
         */
        UILayoutNode* getPrevSibling() const;

        /**
         * Returns the closest next sibling node that is a layout node.
         * @return the closest next sibling node that is a layout node.
         */
        UILayoutNode* getNextSibling() const;

        /**
         * Returns the first child node that is a layout node.
         * @return the first child node that is a layout node.
         */
        UILayoutNode* getFirstChild() const;

        /**
         * Returns the last child node that is a layout node.
         * @return the last child node that is a layout node.
         */
        UILayoutNode* getLastChild() const;

        /**
         * Returns the furthest ancestor node that is a layout node.
         * @return the furthest ancestor node that is a layout node.
         */
        UILayoutNode* getRoot() const;

        const std::shared_ptr<Layout>& getLayout() const {
            return m_layout;
        }

        virtual void setLayout(const std::shared_ptr<Layout>& layout);

        float getMinPreferredWidth() const {
            return m_minPreferredWidth;
        }

        virtual void setMinPreferredWidth(float v);

        float getMinPreferredHeight() const {
            return m_minPreferredHeight;
        }

        virtual void setMinPreferredHeight(float v);

        virtual void setMinPreferredSize(float w, float h);

        float getMaxPreferredWidth() const {
            return m_maxPreferredWidth;
        }

        virtual void setMaxPreferredWidth(float v);

        float getMaxPreferredHeight() const {
            return m_maxPreferredHeight;
        }

        virtual void setMaxPreferredHeight(float v);

        virtual void setMaxPreferredSize(float w, float h);

        float getPreferredWidth() const {
            return m_preferredWidth;
        }

        virtual void setPreferredWidth(float v);

        float getPreferredHeight() const {
            return m_preferredHeight;
        }

        virtual void setPreferredHeight(float v);

        virtual void setPreferredSize(float w, float h);

        virtual void setPreferredSize(float minWidth, float minHeight, float width, float height, float maxWidth, float maxHeight);

    protected:
        void onCalcGeometry(const UINode* parent) override;

        void onCalcGeometry() override;

        /**
         * In addition to updating the geometry and position validity of the child,
         * it also sets the layout validity of the child to false,
         * if this has an invalid geometry.
         */
        void setChildGeometryInvalid(UINode* child) const override;

        /**
         * Sets the geometry and position and the layout to be in valid state.
         */
        void setGeometryValid() override;

        /**
         * Invoked when the position type of a child is changed.
         * It recomputes the layout of the node, since one of its children changed position type.
         * @param child the child that the position changed of.
         */
        void onChildPositionTypeChanged(const UIPositionedNode* child) override;

        void onChildPreferredSizeChanged(const UILayoutNode* child);

        void onResourceChanged(const Layout& layout);

    private:
        std::shared_ptr<Layout> m_layout;
        float m_minPreferredWidth{ 0.0f };
        float m_minPreferredHeight{ 0.0f };
        float m_preferredWidth{ 0.0f };
        float m_preferredHeight{ 0.0f };
        float m_maxPreferredWidth{ FLT_MAX };
        float m_maxPreferredHeight{ FLT_MAX };
        bool m_layoutValid{ false };
        bool m_calculatingPreferredSize{ false };
        bool m_preferredSizeChanged{ false };

        void invalidateLayout();
        void setPreferredSizeChanged();
        
        friend class SharedResource<Layout, UILayoutNode>;
    };


} //namespace algui


#endif //ALGUI_UILAYOUTNODE_HPP
