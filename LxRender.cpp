#include "stdafx.h"
#include "LxPaper.h"
#include "LxRender.h"
#include "LxViewWindow.h"

//context render,only draw text.
LxContexRender::LxContexRender(ComposeDoc* compose_doc, GD_proxy_base* gd_proxy) 
	: compose_doc_(compose_doc), 
	gd_proxy_(gd_proxy) {}
LxContexRender::~LxContexRender() {}

void LxContexRender::DrawSection(CDC* pDC, Section* section)
{
	compose_doc_->draw_section(pDC, section, false);
}

void LxContexRender::ClearSection(CDC* pDC, Section* section)
{
	compose_doc_->draw_section(pDC, section, false,LxPaper::paper_back_color_s);
}

void LxContexRender::DrawDocument(CDC* pDC, Section* section)
{
	compose_doc_->Draw(pDC, section);
}

void LxContexRender::create_caret(int height, int width)
{
	gd_proxy_->create_caret(height, width);
}
void LxContexRender::show_caret(LxCursor* cursor)
{
	gd_proxy_->show_caret(cursor->point_x + ViewWindow::GetViewWindowInstance()->border_width_left -
		ViewWindow::GetViewWindowInstance()->offset_x,
		cursor->point_y - ViewWindow::GetViewWindowInstance()->offset_y);
}
void LxContexRender::hide_caret()
{
	gd_proxy_->hide_caret();
}
ComposeDoc* LxContexRender::get_compose_doc()
{
	return compose_doc_;
}
GD_proxy_base* LxContexRender::get_gd_proxy()
{
	return gd_proxy_;
}

//border render,draw border and the base render which is decorated.
LxBorderRender::LxBorderRender(LxRender* base_render)
	: base_render_(base_render) {}
LxBorderRender::~LxBorderRender()
{
	delete base_render_;
}

void LxBorderRender::DrawPageSpace(CDC* pDC, int bottom_pos, int pages_space)
{
	CRect rect(ViewWindow::GetViewWindowInstance()->border_width_left -
		ViewWindow::GetViewWindowInstance()->offset_x,
		bottom_pos - pages_space,
		ViewWindow::GetViewWindowInstance()->border_width_left + LxPaper::pixel_width -
		ViewWindow::GetViewWindowInstance()->offset_x,
		bottom_pos);
	FlushRect(pDC, &rect, ViewWindow::GetViewWindowInstance()->get_view_back_color());
}

void LxBorderRender::DrawBorder(CDC* pDC)
{
	// draw spaces between pages
	ComposeDoc* compose_doc_ = base_render_->get_compose_doc();
	int view_top = ViewWindow::GetViewWindowInstance()->get_top_pos();
	int view_bottom = ViewWindow::GetViewWindowInstance()->get_bottom_pos();
	int pages_space = ViewWindow::GetViewWindowInstance()->border_height;
	int base_top = ViewWindow::GetViewWindowInstance()->offset_y;
	for (page_iter page_ = compose_doc_->begin(); page_ != compose_doc_->end(); ++page_)
	{
		if ((*page_)->get_top_pos() > view_top /*&& (*page_)->get_top_pos() < view_bottom*/)
		{
			DrawPageSpace(pDC, (*page_)->get_top_pos() - base_top, pages_space);
		}
		if ((*page_)->get_bottom_pos() > view_top && (*page_)->get_bottom_pos() < view_bottom)
		{
			DrawPageSpace(pDC, (*page_)->get_bottom_pos() + pages_space - base_top, pages_space);
			page_++;
			if (page_ == compose_doc_->end())
				break;
			continue;
		}
		if ((*page_)->get_top_pos() >= view_bottom)
			break;
	}
	// draw bottom border if need
	int doc_bottom_pos = (*(--compose_doc_->end()))->get_bottom_pos() /*+ pages_space*/ - 
		ViewWindow::GetViewWindowInstance()->offset_y;
	if (doc_bottom_pos < ViewWindow::GetViewWindowInstance()->height)
	{
		CRect rect_b(ViewWindow::GetViewWindowInstance()->border_width_left, doc_bottom_pos,
			ViewWindow::GetViewWindowInstance()->width - ViewWindow::GetViewWindowInstance()->border_width_right,
			ViewWindow::GetViewWindowInstance()->height);
		FlushRect(pDC, &rect_b, ViewWindow::GetViewWindowInstance()->get_view_back_color());
	}
	// draw left and right border
	CRect rect_l(0, 0, ViewWindow::GetViewWindowInstance()->border_width_left, 
		ViewWindow::GetViewWindowInstance()->height);
	CRect rect_r(ViewWindow::GetViewWindowInstance()->width - ViewWindow::GetViewWindowInstance()->border_width_right,
		0, ViewWindow::GetViewWindowInstance()->width, ViewWindow::GetViewWindowInstance()->height);
	FlushRect(pDC, &rect_l, ViewWindow::GetViewWindowInstance()->get_view_back_color());
	FlushRect(pDC, &rect_r, ViewWindow::GetViewWindowInstance()->get_view_back_color());
}

void LxBorderRender::DrawSection(CDC* pDC, Section* section)
{
	base_render_->DrawSection(pDC, section);
}

void LxBorderRender::ClearSection(CDC* pDC, Section* section)
{
	base_render_->ClearSection(pDC, section);
}

void LxBorderRender::DrawDocument(CDC* pDC, Section* section)
{
	base_render_->DrawDocument(pDC, section);
	DrawBorder(pDC);
}

void LxBorderRender::create_caret(int height, int width)
{
	base_render_->create_caret(height, width);
}
void LxBorderRender::show_caret(LxCursor* cursor)
{
	base_render_->show_caret(cursor);
}
void LxBorderRender::hide_caret()
{
	base_render_->hide_caret();
}
ComposeDoc* LxBorderRender::get_compose_doc()
{
	return base_render_->get_compose_doc();
}
GD_proxy_base* LxBorderRender::get_gd_proxy()
{
	return base_render_->get_gd_proxy();
}

//scroll render,draw scrollbar and the base render which is decorated.
LxScrollRender::LxScrollRender(LxRender* base_render)
	: base_render_(base_render) {}
LxScrollRender::~LxScrollRender()
{
	delete base_render_;
}

void LxScrollRender::set_scroll_size_total(int width_total, int height_total)
{
	get_gd_proxy()->set_scroll_size_total(width_total, height_total);
}
void LxScrollRender::set_scroll_pos(int hscroll_pos, int vscroll_pos)
{
	get_gd_proxy()->set_scroll_pos(hscroll_pos, vscroll_pos);
}

void LxScrollRender::DrawSection(CDC* pDC, Section* section)
{
	base_render_->DrawSection(pDC, section);
}

void LxScrollRender::ClearSection(CDC* pDC, Section* section)
{
	base_render_->ClearSection(pDC, section);
}

void LxScrollRender::DrawDocument(CDC* pDC, Section* section)
{
	base_render_->DrawDocument(pDC, section);
}

void LxScrollRender::create_caret(int height, int width)
{
	base_render_->create_caret(height, width);
}
void LxScrollRender::show_caret(LxCursor* cursor)
{
	base_render_->show_caret(cursor);
}
void LxScrollRender::hide_caret()
{
	base_render_->hide_caret();
}
ComposeDoc* LxScrollRender::get_compose_doc()
{
	return base_render_->get_compose_doc();
}
GD_proxy_base* LxScrollRender::get_gd_proxy()
{
	return base_render_->get_gd_proxy();
}